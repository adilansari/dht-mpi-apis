
/**
   Extremely simple singly-linked list of int-keyed data items.
 */

#ifndef ILIST_H
#define ILIST_H

#include <stdio.h>
#include <stdlib.h>

#include <mpi_tools.h>

// Maximum size of a ilist datum
#define ILIST_MAX_DATUM 100

struct ilist_item
{
  int   key;
  void* data;
  struct ilist_item* next;
};

struct ilist
{
  struct ilist_item* head;
  struct ilist_item* tail;
  int size;
};

struct ilist* ilist_create(void);

void ilist_append(struct ilist* target, int key, void* data);

bool ilist_add(struct ilist* target, int key, void* data);

#define ilist_push(target, key, data) ilist_add(target, key, data)

//struct ilist_item* ilist_replace(struct ilist* target,
//                                 int key, void* data);

bool ilist_contains(struct ilist* target, int key);

bool ilist_matches(struct ilist* target,
                   int (*cmp)(void*,void*), void* data);

struct ilist_item* ilist_ordered_insert(struct ilist* target,
                                        int key, void* data);
struct ilist_item*
ilist_ordered_insert_unique(struct ilist* target,
                            int (*cmp)(void*,void*),
                            int key, void* data);

void* ilist_pop(struct ilist* target);

void* ilist_poll(struct ilist* target);

void* ilist_get(struct ilist* target, int i);

void* ilist_search(struct ilist* target, int key);

void ilist_free(struct ilist* target);

void ilist_destroy(struct ilist* target);

void* ilist_remove(struct ilist* target, int key);

//// Output methods...
void ilist_dump(char* (f)(void*), struct ilist* target);
void ilist_printf(char* format, struct ilist* target);
void ilist_fdump(FILE* file, char* (f)(void*), struct ilist* target);
void ilist_fprintf(FILE* file, char* format, struct ilist* target);
void ilist_dumpkeys(struct ilist* target);
void ilist_xdumpkeys(struct ilist* target);
void ilist_fdumpkeys(FILE* file, struct ilist* target);
void ilist_output(char* (*f)(void*), struct ilist* target);
int  ilist_snprintf(char* str, size_t size,
                    char* format, struct ilist* target);
int ilist_marshal(char* str, size_t size,
                  char* (f)(void*), struct ilist* target);
char* ilist_serialize_ptrs(struct ilist* target);


#endif
