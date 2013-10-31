/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_cmpi.h"
#include "adioi.h"

void ADIOI_CMPI_WriteContig(ADIO_File fd, void *buf, int count, 
			      MPI_Datatype datatype, int file_ptr_type,
			      ADIO_Offset offset, ADIO_Status *status, int
			      *error_code)
{
    int myrank, nprocs, datatype_size;

    printf("CMPI_WriteContig() \n"); // CMPI_NOTE
    
    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    MPI_Type_size(datatype, &datatype_size);
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_WriteContig called on %s\n", myrank, 
	    nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    writing (buf = %p, loc = %lld, sz = %lld)\n",
	    myrank, nprocs, buf, (long long) offset, 
	    (long long)datatype_size * (long long)count);

    int blocks = cmpi_io_blocks(fd->filename); 
    SHOW_I(blocks); 
    
    if (file_ptr_type != ADIO_EXPLICIT_OFFSET)
    {
        int block   = fd->fp_ind / CMPI_IO_BLOCK; 
        int partial = fd->fp_ind % CMPI_IO_BLOCK;
        int length  = CMPI_IO_BLOCK - partial;
        char key[CMPI_IO_MAX_KEY];  
        char value[CMPI_IO_BLOCK];

        sprintf(key, "%s[%i]", fd->filename, block);
        strncpy(value,  buf, length); 
        cmpi_update(key, value, length); 
        
	fd->fp_ind += datatype_size * count;
	fd->fp_sys_posn = fd->fp_ind;
	FPRINTF(stdout, "[%d/%d]    new file position is %lld\n", myrank, 
		nprocs, (long long) fd->fp_ind);
    }
    else {
	fd->fp_sys_posn = offset + datatype_size * count;
    }

#ifdef HAVE_STATUS_SET_BYTES
    MPIR_Status_set_bytes(status, datatype, datatype_size * count);
#endif
}

void ADIOI_CMPI_WriteStrided(ADIO_File fd, void *buf, int count,
			       MPI_Datatype datatype, int file_ptr_type,
			       ADIO_Offset offset, ADIO_Status *status,
			       int *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_WriteStrided called on %s\n", 
	    myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_WriteStrided\n", 
	    myrank, nprocs);

    ADIOI_GEN_WriteStrided(fd, buf, count, datatype, file_ptr_type, offset, 
			   status, error_code);
}
