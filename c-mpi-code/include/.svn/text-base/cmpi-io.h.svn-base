
/**
   MPI-IO implementation on CMPI.
   Build with Makefile target cmpi-io.  
   Link with target MPI-IO application and set
   LD_PRELOAD to include libcmpi-io.so .
*/

#ifndef CMPI_IO_H
#define CMPI_IO_H

#include <stdlib.h> 

#include "cmpi.h"

#define CMPI_IO_BLOCK_SIZE 10

typedef struct
{
  
} CMPI_Block; 

struct cmpi_file
{
  char* filename; 
  
  /**
     Number of blocks
  */
  int blocks;
  
  /**
     Map block numbers to MPI ranks.
  */
  struct ltable* block_table; 
};

typedef struct cmpi_file* CMPI_File; 

extern CMPI_File CMPI_FILE_NULL;

void cmpi_io_init(void);

#endif
