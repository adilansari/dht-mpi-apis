
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <xtree.h>
#include <ilist.h>

/**
   @return The ith bit of id.
*/
int
bit(int i, xtree_id id)
{
  // printf("shift: %i\n", (1 << i));

  return !! ((1 << i) & id);
}

struct xtree*
xtree_create()
{
  struct xtree* result;

  result = malloc(sizeof(struct xtree));
  result->size = 0;
  result->root = NULL;

  return result;
}

void
xtree_add(struct xtree* target, xtree_id id, void* data)
{
  xtree_object*  new_object;
  xtree_object** p;
  bool done;
  int  depth;

  new_object = malloc(sizeof(xtree_object));
  new_object->tag = XTREE_ITEM;
  new_object->thing.item.id   = id;
  new_object->thing.item.data = data;

  if (target->size == 0)
  {
    target->root = new_object;
    target->size++;
    return;
  }

  p = &(target->root);
  depth = XTREE_DEPTH-1;
  done = false;
  while (!done)
  {
    if (*p == NULL)
    {
      printf("inserting at NULL stub: depth: %i\n", depth);
      *p = new_object;
      target->size++;
      break;
    }
    else if ((*p)->tag == XTREE_ITEM)
    {
      xtree_replace(target, depth, *p, new_object);
      target->size++;
      break;
    }
    else if ((*p)->tag == XTREE_NODE)
    {
      if (bit(depth, id) == 0)
        p = &((*p)->thing.node.zero);
      else
        p = &((*p)->thing.node.one);
      depth--;
    }
  }
}

void
xtree_replace(struct xtree* target, int depth,
              xtree_object* old_object, xtree_object* new_object)
{
  xtree_item old_item = {0};
  xtree_object* p;

  xtree_id old_id;
  xtree_id new_id;

  xtree_object* new_node;

  // Shortcuts...
  old_id = old_object->thing.item.id;
  new_id = new_object->thing.item.id;

  old_item.id   = old_object->thing.item.id;
  old_item.data = old_object->thing.item.data;

  old_object->tag = XTREE_NODE;
  old_object->thing.node.zero = NULL;
  old_object->thing.node.one  = NULL;

  printf("xtree_replace(%i)\n", depth);
  printf("old %i: \n", old_id);
  printf("new %i: \n", new_id);

  p = old_object;

  old_object = malloc(sizeof(xtree_object));
  old_object->tag = XTREE_ITEM;
  old_object->thing.item = old_item;

  while (true)
  {
    if (bit(depth,old_id) != bit(depth,new_id))
    {
      printf("diff at: %i\n", depth);

      if (bit(depth,old_id) == 0)
      {
        p->thing.node.zero = old_object;
        p->thing.node.one  = new_object;
      }
      else
      {
        p->thing.node.zero = new_object;
        p->thing.node.one  = old_object;
      }
      break;
    }
    else
    {
      // printf("new_node\n");
      new_node = malloc(sizeof(xtree_object));
      new_node->tag = XTREE_NODE;
      new_node->thing.node.zero = NULL;
      new_node->thing.node.one  = NULL;
      if (bit(depth,old_item.id) == 0)
        p->thing.node.zero = new_node;
      else
        p->thing.node.one = new_node;
      p = new_node;
    }
    depth--;
  }
}

/**
*/
void
xtree_find(xtree_object* p, xtree_id id, int count,
           int depth, struct ilist* objects)
{
  if (p == NULL)
  {
    return;
  }

  if (p->tag == XTREE_ITEM)
  {
    ilist_ordered_insert(objects, id ^ p->thing.item.id, p);
    return;
  }

  if (bit(depth, id) == 0)
  {
    xtree_find(p->thing.node.zero, id, count, depth-1, objects);
    if (objects->size >= count)
      return;
    xtree_find(p->thing.node.one, id, count, depth-1, objects);
  }
  else
  {
    xtree_find(p->thing.node.one, id, count, depth-1, objects);
    if (objects->size >= count)
      return;
    xtree_find(p->thing.node.zero, id, count, depth-1, objects);
  }
}

struct inlist*
xtree_query(struct xtree* target, xtree_id id, int count)
{
  struct inlist* result;
  struct ilist* objects;

  struct ilist_item* item;
  xtree_object* object;

  objects = ilist_create();
  xtree_find(target->root, id, count, XTREE_DEPTH-1, objects);

  result = inlist_create();
  for (item = objects->head; item; item = item->next)
  {
    object = (xtree_object*) item->data;
    inlist_add(result, object->thing.item.id);
  }

  return result;
}

/**
   Return the data object for the matching id.
*/
void*
xtree_search(struct xtree* target, xtree_id id)
{
  void* result;
  struct ilist* objects = ilist_create();
  xtree_object* object;

  result = NULL;

  xtree_find(target->root, id, 1, XTREE_DEPTH-1, objects);

  if (objects->size > 0)
  {
    object = (xtree_object*) objects->head->data;
    if (object->thing.item.id == id)
      result = object->thing.item.data;
  }

  return result;
}

void
xtree_printf_item(xtree_item* item, int depth)
{
  printf(" %i", item->id);
}

void
xtree_printf_node(xtree_object* object, int depth)
{
  char spaces[256];

  memset(spaces, ' ', XTREE_DEPTH-depth);
  spaces[XTREE_DEPTH-depth] = '\0';

  // printf("depth: %i\n", depth)

  if (object != NULL)
  {
    if (object->tag == XTREE_ITEM)
    {
      printf("ITEM: ");
      xtree_printf_item(&(object->thing.item), depth);
    }
    else
    {
      if (object->thing.node.zero != NULL)
      {
        printf("\n%s0 ", spaces);
        xtree_printf_node(object->thing.node.zero, depth-1);
      }
      if (object->thing.node.one != NULL)
      {
        printf("\n%s1 ", spaces);
        xtree_printf_node(object->thing.node.one, depth-1);
      }
    }
  }
}

void
xtree_printf(struct xtree* target)
{
  if (target->size > 0)
    xtree_printf_node(target->root, XTREE_DEPTH-1);
  else
    printf("xtree: empty");
  printf("\n");
}

