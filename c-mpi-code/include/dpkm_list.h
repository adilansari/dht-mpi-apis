
/**
 * Doubly-linked list with two data items.
 * DPKM_LIST =
 * "Doubly-linked" "extra Pointer" "Keyed" "capacity-liMited" LIST
 * */

#ifndef DPKM_LIST_H
#define DPKM_LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mpi_tools.h>

#include "keyvalue.h"

// Maximum size of a klist datum
#define DPKM_LIST_MAX_DATUM 100

struct dpkm_list_item
{
  void* ptr;
  char* key;
  char* data;
  int   length;
  struct dpkm_list_item* prev;
  struct dpkm_list_item* next;
};

struct dpkm_list
{
  struct dpkm_list_item* head;
  struct dpkm_list_item* tail;
  int size;
  int limit;
};

struct dpkm_list* dpkm_list_create(int limit);

struct dpkm_list_item* dpkm_list_add(struct dpkm_list* target,
                                     char* key, void* data, int length);

// #define dpkm_list_push(target, key, data, length) dpkm_list_add(target, key, data, length)

void dpkm_list_pushback(struct dpkm_list* target,
                        struct dpkm_list_item* item);

struct dpkm_list_item* dpkm_list_append(struct dpkm_list* target,
                                        struct dpkm_list_item* item);

struct dpkm_list_item* dpkm_list_search(struct dpkm_list* target,
                                        char* key);

void* dpkm_list_inspect(struct dpkm_list* target, void* data, size_t n);

bool dpkm_list_remove(struct dpkm_list* target, void* data);

bool dpkm_list_erase(struct dpkm_list* target, void* data, size_t n);

void dpkm_list_excise(struct dpkm_list* target,
                      struct dpkm_list_item* item);

struct dpkm_list* dpkm_list_select(struct dpkm_list* target,
                         int (*cmp)(void*,void*), void* arg);

bool dpkm_list_remove_where(struct dpkm_list* target,
                       int (*cmp)(void*,void*), void* arg);

struct dpkm_list* dpkm_list_pop_where(struct dpkm_list* target,
                                      int (*cmp)(void*,void*), void* arg);

void dpkm_list_transplant(struct dpkm_list* target, struct dpkm_list* segment);

void dpkm_list_clear(struct dpkm_list* target);

void dpkm_list_clobber(struct dpkm_list* target);

void* dpkm_list_pop(struct dpkm_list* target);

void* dpkm_list_peek(struct dpkm_list* target);

struct dpkm_list_item* dpkm_list_poll(struct dpkm_list* target);

void* dpkm_list_random(struct dpkm_list* target);

struct dpkm_list_item* dpkm_list_ordered_insert(struct dpkm_list* target,
                                      int (*cmp)(void*,void*),
                                      void* data);

bool dpkm_list_contains(struct dpkm_list* target,
                   int (*cmp)(void*,void*), void* data);

void dpkm_list_output(char* (*f)(void*), struct dpkm_list* target);

void dpkm_list_free(struct dpkm_list* target);

void dpkm_list_destroy(struct dpkm_list* target);

void dpkm_list_printf(char* format, struct dpkm_list* target);

void dpkm_list_fprintf(FILE* file, char* format,
                       struct dpkm_list* target);

void dpkm_list_printdata(struct dpkm_list* target);
void dpkm_list_fprintdata(FILE* file, struct dpkm_list* target);


int dpkm_list_tostring(char* str, size_t size,
                       char* format, struct dpkm_list* target);


#endif
