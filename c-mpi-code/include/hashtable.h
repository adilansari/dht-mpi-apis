/* 2001 Clemson University and The University of Chicago
 *
 * See COPYING in top-level directory.
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <openssl/sha.h>

#include <mpi_tools.h>

#include "klist.h"

struct hashtable
{
  struct klist** array;
  int capacity;
  int size;
};

// inline int quickhash_string_hash(char* key, int capacity);
int hash_string(char* key, int capacity);

/**
   Compress SHA-1 down to a smaller address space,
   namely, 4 bytes.
*/
int SHA1_mod(char* data);

struct hashtable* hashtable_create(int capacity);

bool  hashtable_add(struct hashtable *target, char* key, void* data);

void* hashtable_search(struct hashtable* target, char* key);

void  hashtable_dump(char* format, struct hashtable* target);

void  hashtable_free(struct hashtable* target);

void  hashtable_destroy(struct hashtable* target);

/**
   Simply printf all keys in each klist.
*/
void  hashtable_dumpkeys(struct hashtable* target);

#endif

