/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#include "ad_cmpi.h"
#include "adioi.h"
#ifdef ROMIO_BGL
#include "../ad_bgl/ad_bgl.h"
#endif
void ADIOI_CMPI_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code)
{
    int myrank, nprocs;

    *error_code = MPI_SUCCESS;

    MPI_Comm_size(fd->comm, &nprocs);
    MPI_Comm_rank(fd->comm, &myrank);
    FPRINTF(stdout, "[%d/%d] ADIOI_CMPI_SetInfo called on %s\n", 
	    myrank, nprocs, fd->filename);
    FPRINTF(stdout, "[%d/%d]    calling ADIOI_GEN_SetInfo\n", 
	    myrank, nprocs);

#ifdef ROMIO_BGL   /* BlueGene support for pvfs through ufs */
    /* BlueGene hack: force testfs to mimic BlueGene hints */
    ADIOI_BGL_SetInfo(fd, users_info, error_code);
#else
    ADIOI_GEN_SetInfo(fd, users_info, error_code);
#endif
}
