/* 2001 Clemson University and The University of Chicago
 *
 * See COPYING in top-level directory.
 *
 * Modified for Grayskull by Justin M Wozniak <wozniak@mcs.anl.gov>
 */

#ifndef ITABLE_H
#define ITABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "ilist.h"

struct itable
{
  struct ilist** array;
  int capacity;
  int size;
};

int hash_int(int key, int capacity);

struct itable* itable_create(int capacity);

bool  itable_add(struct itable* target, int key, void* data);

void* itable_search(struct itable* target, int key);

void* itable_remove(struct itable* target, int key);

void  itable_free(struct itable* target);

void itable_destroy(struct itable* target);

void  itable_dump(char* format, struct itable* target);

int itable_tostring(char* str, size_t size,
                      char* format, struct itable* target);

#endif

