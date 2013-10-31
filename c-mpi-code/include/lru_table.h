/* 2001 Clemson University and The University of Chicago
 *
 * See COPYING in top-level directory.
 */

#ifndef LRU_TABLE_H
#define LRU_TABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <mpi_tools.h>
#include <gossip.h>

// Use hash functions from hashtable.h
#include "hashtable.h"
#include "dpkm_list.h"
#include "keyvalue.h"

struct lru_table
{
  struct dpkm_list** array;
  struct dpkm_list*  lru;
  int capacity;
  int size;
  int limit;
};

struct lru_table* lru_table_create(int capacity, int limit);

struct keyvalue* lru_table_add(struct lru_table *table,
                               char* key, void* data, int length);

bool lru_table_update(struct lru_table *table,
                      char* key, char* data, int offset, int length);

struct keyvalue* lru_table_search(struct lru_table* table, char* key);

struct keyvalue* lru_table_poll(struct lru_table* table);

void lru_table_printf(char* format, struct lru_table* target);

void lru_table_fprintf(FILE* file, char* format, struct lru_table* target);

void lru_table_printdata(struct lru_table* target);

void lru_table_fprintdata(FILE* file, struct lru_table* target);

void lru_table_destroy(struct lru_table* target);

void lru_table_free(struct lru_table* target);

#endif
