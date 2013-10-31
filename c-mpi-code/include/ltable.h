/* 2001 Clemson University and The University of Chicago
 *
 * See COPYING in top-level directory.
 */

#ifndef LTABLE_H
#define LTABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <mpi_tools.h>

#include "llist.h"

struct ltable
{
  struct llist** array;
  int capacity;
  int size;
};

int hash_long(long key, int capacity);

struct ltable* ltable_create(int capacity);

bool  ltable_add(struct ltable *table, long key, void* data);

void* ltable_search(struct ltable* table, long key);

void* ltable_remove(struct ltable* table, long key);

void  ltable_dump(char* format, struct ltable* target);

int ltable_tostring(char* str, size_t size,
                      char* format, struct ltable* target);

#endif

