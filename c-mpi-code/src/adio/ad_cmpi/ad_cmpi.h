/* -*- Mode: C; c-basic-offset:4 ; -*- */
/* 
 *
 *   Copyright (C) 2001 University of Chicago. 
 *   See COPYRIGHT notice in top-level directory.
 */

#ifndef AD_TESTFS_INCLUDE
#define AD_TESTFS_INCLUDE

#include <unistd.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <fcntl.h>
#include "adio.h"

#include "cmpi.h" 

void ADIOI_CMPI_Open(ADIO_File fd, int *error_code);
void ADIOI_CMPI_Close(ADIO_File fd, int *error_code);
void ADIOI_CMPI_ReadContig(ADIO_File fd, void *buf, int count, 
			     MPI_Datatype datatype, int file_ptr_type,
			     ADIO_Offset offset, ADIO_Status *status, int
			     *error_code);
void ADIOI_CMPI_WriteContig(ADIO_File fd, void *buf, int count, 
			      MPI_Datatype datatype, int file_ptr_type,
			      ADIO_Offset offset, ADIO_Status *status, int
			      *error_code);   
void ADIOI_CMPI_IwriteContig(ADIO_File fd, void *buf, int count, 
			       MPI_Datatype datatype, int file_ptr_type,
			       ADIO_Offset offset, ADIO_Request *request, int
			       *error_code);   
void ADIOI_CMPI_IreadContig(ADIO_File fd, void *buf, int count, 
			      MPI_Datatype datatype, int file_ptr_type,
			      ADIO_Offset offset, ADIO_Request *request, int
			      *error_code);   
int ADIOI_CMPI_ReadDone(ADIO_Request *request, ADIO_Status *status, int
			  *error_code);
int ADIOI_CMPI_WriteDone(ADIO_Request *request, ADIO_Status *status, int
			   *error_code);
void ADIOI_CMPI_ReadComplete(ADIO_Request *request, ADIO_Status *status, int
			       *error_code); 
void ADIOI_CMPI_WriteComplete(ADIO_Request *request, ADIO_Status *status,
				int *error_code); 
void ADIOI_CMPI_Fcntl(ADIO_File fd, int flag, ADIO_Fcntl_t *fcntl_struct, 
			int *error_code); 
void ADIOI_CMPI_WriteStrided(ADIO_File fd, void *buf, int count,
			       MPI_Datatype datatype, int file_ptr_type,
			       ADIO_Offset offset, ADIO_Status *status,
			       int *error_code);
void ADIOI_CMPI_ReadStrided(ADIO_File fd, void *buf, int count,
			      MPI_Datatype datatype, int file_ptr_type,
			      ADIO_Offset offset, ADIO_Status *status, int
			      *error_code);
void ADIOI_CMPI_WriteStridedColl(ADIO_File fd, void *buf, int count,
				   MPI_Datatype datatype, int file_ptr_type,
				   ADIO_Offset offset, ADIO_Status *status, int
				   *error_code);
void ADIOI_CMPI_ReadStridedColl(ADIO_File fd, void *buf, int count,
				  MPI_Datatype datatype, int file_ptr_type,
				  ADIO_Offset offset, ADIO_Status *status, int
				  *error_code);
void ADIOI_CMPI_IreadStrided(ADIO_File fd, void *buf, int count,
			       MPI_Datatype datatype, int file_ptr_type,
			       ADIO_Offset offset, ADIO_Request *request, int
			       *error_code);
void ADIOI_CMPI_IwriteStrided(ADIO_File fd, void *buf, int count,
				MPI_Datatype datatype, int file_ptr_type,
				ADIO_Offset offset, ADIO_Request *request, int
				*error_code);
void ADIOI_CMPI_Flush(ADIO_File fd, int *error_code);
void ADIOI_CMPI_Resize(ADIO_File fd, ADIO_Offset size, int *error_code);
ADIO_Offset ADIOI_CMPI_SeekIndividual(ADIO_File fd, ADIO_Offset offset, 
					int whence, int *error_code);
void ADIOI_CMPI_SetInfo(ADIO_File fd, MPI_Info users_info, int *error_code);
void ADIOI_CMPI_Get_shared_fp(ADIO_File fd, int size, 
				ADIO_Offset *shared_fp, 
				int *error_code);
void ADIOI_CMPI_Set_shared_fp(ADIO_File fd, ADIO_Offset offset, 
				int *error_code);
void ADIOI_CMPI_Delete(char *filename, int *error_code);

#endif




