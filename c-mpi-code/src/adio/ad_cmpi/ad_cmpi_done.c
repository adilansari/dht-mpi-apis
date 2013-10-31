/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_cmpi.h"
#include "adioi.h"

int ADIOI_CMPI_ReadDone(ADIO_Request *request, ADIO_Status *status, int
			  *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_ReadDone called on ADIO_REQUEST_NULL\n", 
	myrank, nprocs);
    return 1;
}


int ADIOI_CMPI_WriteDone(ADIO_Request *request, ADIO_Status *status, int
			   *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
    FPRINTF(stdout, 
		"[%d/%d] ADIOI_CMPI_WriteDone called on ADIO_REQUEST_NULL\n", 
		myrank, nprocs);
    return 1;
}
