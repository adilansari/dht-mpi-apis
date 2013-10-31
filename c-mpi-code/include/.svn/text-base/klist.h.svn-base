
/** Extremely simple singly-linked klist of keyed data items.
 */

#ifndef KLIST_H
#define KLIST_H

#include <stdio.h>
#include <stdlib.h>

#include <mpi_tools.h>

// Maximum size of a klist datum
#define KLIST_MAX_DATUM 100

struct klist_item
{
  char* key;
  void* data;
  struct klist_item* next;
};

struct klist
{
  struct klist_item* head;
  struct klist_item* tail;
  int size;
};

struct klist* klist_create(void);

struct klist_item* klist_add(struct klist* target,
                           char* key, void* data);

void klist_free(struct klist* target);

void klist_destroy(struct klist* target);

void klist_dump(char* format, struct klist* target);

void klist_dumpkeys(struct klist* target);

int klist_tostring(char* str, size_t size,
                  char* format, struct klist* target);

#endif
