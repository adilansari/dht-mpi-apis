
#ifndef DISKSIM_H
#define DISKSIM_H

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

#include <disksim_interface.h>
#include <disksim_rand48.h>

#include <mpi.h> 

#include <cmpi-config.h> 

#include <mpi_tools.h> 

//// Public API:
void disksim_init(void); 
void cmpi_disk_storepair(char* key, char* value, int length);
int cmpi_disk_loadpair(char* key,  char** data); 

void emulate_read(long block,  char** data, int size); 
void emulate_write(long block, char* data,  int size);

/**
   Compute the disk access delay using Disksim.
   @return The delay in us as as double, -1 on error.   
*/
double compute_read(long block, char** data, int size);

/**
   Compute the disk access delay using Disksim.
   @return The delay in us as as double, -1 on error.   
*/
double compute_write(long block, char* data, int size); 

#endif
