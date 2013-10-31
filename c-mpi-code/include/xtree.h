
/**
 * XTREE: XOR-metric lookup tree.
 * */

#ifndef XTREE_H
#define XTREE_H

#include <inlist.h>

typedef int xtree_id;
#define XTREE_DEPTH (sizeof(xtree_id)*8)

typedef struct xtree_object_s xtree_object;
typedef union  xtree_thing_u  xtree_thing;

typedef enum
{
  XTREE_NODE,
  XTREE_ITEM
} xtree_tag;

typedef struct xtree_item_s
{
  xtree_id id;
  void* data;
  xtree_object* smaller;
  xtree_object* bigger;
} xtree_item;

typedef struct xtree_node_s
{
  xtree_object* zero;
  xtree_object* one;
} xtree_node;

union xtree_thing_u
{
  xtree_node node;
  xtree_item item;
};

struct xtree_object_s
{
  xtree_tag  tag;
  xtree_thing thing;
};

struct xtree
{
  int size;
  xtree_object* root;
};

struct xtree* xtree_create();

void xtree_add(struct xtree* target, xtree_id id, void* data);

void* xtree_search(struct xtree* target, xtree_id id);

struct inlist* xtree_query(struct xtree* target,
                           xtree_id id, int count);

void xtree_printf(struct xtree* target);

//// Internals:

void xtree_replace(struct xtree* target, int depth,
                   xtree_object* old_object,
                   xtree_object* new_object);

#endif
