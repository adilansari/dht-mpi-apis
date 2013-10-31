/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_cmpi.h"
#include "adioi.h"

/* ADIOI_CMPI_IreadContig()
 *
 * Implemented by immediately calling ReadContig()
 */
void ADIOI_CMPI_IreadContig(ADIO_File fd, void *buf, int count, 
			      MPI_Datatype datatype, int file_ptr_type,
			      ADIO_Offset offset, ADIO_Request *request, int
			      *error_code)
{
    ADIO_Status status;
    int myrank, nprocs, typesize, len;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    MPI_Type_size(datatype, &typesize);
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_IreadContig called on %s\n", 
	    myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_CMPI_ReadContig\n", 
	    myrank, nprocs);

    len = count * typesize;
    ADIOI_CMPI_ReadContig(fd, buf, len, MPI_BYTE, file_ptr_type, 
			    offset, &status, error_code);

    MPIO_Completed_request_create(&fd, len, error_code, request);
}

void ADIOI_CMPI_IreadStrided(ADIO_File fd, void *buf, int count,
			       MPI_Datatype datatype, int file_ptr_type,
			       ADIO_Offset offset, ADIO_Request *request, int
			       *error_code)
{
    ADIO_Status status;
    int myrank, nprocs;
    int typesize;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    MPI_Type_size(datatype, &typesize);
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_IreadStrided called on %s\n", 
	    myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_CMPI_ReadStrided\n", 
	    myrank, nprocs);

    ADIOI_CMPI_ReadStrided(fd, buf, count, datatype, file_ptr_type, 
			     offset, &status, error_code); 
    MPIO_Completed_request_create(&fd, count*typesize, error_code, request);
}

