
#ifndef KEYVALUE_H
#define KEYVALUE_H

#include <mpi_tools.h> 

struct keyvalue
{
  char* key;
  int   length; 
  char* data;
};

void keyvalue_destroy(struct keyvalue* kv); 

#endif
