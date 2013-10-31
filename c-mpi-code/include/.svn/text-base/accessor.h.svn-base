
#ifndef ACCESSOR_H
#define ACCESSOR_H

#include <stdbool.h>
#include <stdio.h>

#include <cmpi-types.h>

extern FILE* to_cmpi;
extern FILE* from_cmpi;

void accessor_fifo_setup(char* dir);

bool accessor_fifo_init(void);

void accessor_fifo_close(void);

CMPI_RETURN accessor_put(char* key, void* data, int length);

#endif
