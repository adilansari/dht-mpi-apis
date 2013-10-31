/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_cmpi.h"
#include "adioi.h"

void ADIOI_CMPI_Open(ADIO_File fd, int *error_code)
{
    int myrank, nprocs;

    printf("CMPI_Open \n"); // CMPI_NOTE
    
    fd->fd_sys = 1;
    fd->fd_direct = -1;
    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_Open called on %s\n", myrank, 
	    nprocs, fd->filename);

    if (fd->access_mode & ADIO_CREATE)
    {
        char* value; 
        cmpi_get(fd->filename, &value);
        if (value == NULL)
        {
            cmpi_put(fd->filename, "FILE 0"); 
        }
    } 
}
