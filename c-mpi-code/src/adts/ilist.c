
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "ilist.h"

struct ilist*
ilist_create()
{
  // NOTE_F;
  struct ilist* new_ilist = malloc(sizeof(struct ilist));
  if (! new_ilist)
    return NULL;
  new_ilist->head = NULL;
  new_ilist->tail = NULL;
  new_ilist->size = 0;
  return new_ilist;
}

void
ilist_append(struct ilist* target, int key, void* data)
{
  struct ilist_item* new_item = malloc(sizeof(struct ilist_item));
  assert(new_item);

  new_item->key  = key;
  new_item->data = data;
  new_item->next = NULL;
  if (target->size == 0)
  {
    target->head = new_item;
    target->tail = new_item;
  }
  else
  {
    target->tail->next = new_item;
  }
  target->tail = new_item;
  target->size++;
}

/**
   Set data
 */
bool
ilist_set(struct ilist* target, int key, void* data)
{
  struct ilist_item* item;
  for (item = target->head; item; item = item->next)
    if (item->key == key)
    {
      item->data = data;
      return true;
    }
  return false;
}

/**
   Add key/data pair to table.
   If key exists, do nothing and return false
*/
bool
ilist_add(struct ilist* target, int key, void* data)
{
  if (ilist_contains(target, key))
    return false;
  ilist_append(target, key, data);
  return true;
}

bool
ilist_contains(struct ilist* target, int key)
{
  struct ilist_item* item;
  for (item = target->head; item; item = item->next)
    if (item->key == key)
      return true;
  return false;
}

bool
ilist_matches(struct ilist* target, int (*cmp)(void*,void*), void* data)
{
  // NOTE_F;

  struct ilist_item* item;
  for (item = target->head; item; item = item->next)
    if (cmp(item->data, data) == 0)
      return true;
  return false;
}

/**
   Insert into list so that keys are in order from smallest at head
   to largest at tail.
*/
struct ilist_item*
ilist_ordered_insert(struct ilist* target, int key, void* data)
{
  // NOTE_F;

  struct ilist_item* new_item = malloc(sizeof(struct ilist_item));
  if (! new_item)
    return NULL;
  new_item->key  = key;
  new_item->data = data;
  new_item->next = NULL;

  if (target->size == 0)
  {
    target->head = new_item;
    target->tail = new_item;
  }
  else
  {
    struct ilist_item* item = target->head;
    // Are we the new head?
    if (key < target->head->key)
    {
      new_item->next = target->head;
      target->head   = new_item;
    }
    else
    {
      do
      {
        // Are we inserting after this item?
        if (item->next == NULL)
        {
          item->next   = new_item;
          target->tail = new_item;
          break;
        }
        else
        {
          if (key < item->next->key)
          {
            new_item->next = item->next;
            item->next = new_item;
            break;
          }
        }
      } while ((item = item->next));
    }
  }
  target->size++;
  return new_item;
}

/**
   Does nothing if the key/data pair are in the list.
   Ordered smallest to largest.
   @return NULL iff the key/data pair are in the list.
   Could optimize to only malloc if insertion point is found.
*/
struct ilist_item*
ilist_ordered_insert_unique(struct ilist* target,
                            int (*cmp)(void*,void*),
                            int key, void* data)
{
  struct ilist_item* new_item = malloc(sizeof(struct ilist_item));
  if (! new_item)
    return NULL;
  new_item->key  = key;
  new_item->data = data;
  new_item->next = NULL;

  if (target->size == 0)
  {
    target->head = new_item;
    target->tail = new_item;
  }
  else
  {
    struct ilist_item* item = target->head;
    // Are we the new head?
    if (key < target->head->key)
    {
      new_item->next = target->head;
      target->head   = new_item;
    }
    // Are we equal to the head?
    else if (key == target->head->key &&
             cmp(data, target->head->data) == 0)
    {
      free(new_item);
      return NULL;
    }
    else
    {
      do
      {
        // Are we inserting after this item?
        if (item->next == NULL)
        {
          item->next   = new_item;
          target->tail = new_item;
          break;
        }
        else
        {
          if (key == item->next->key &&
              cmp(data, item->next->data) == 0)
          {
            free(new_item);
            return NULL;
          }
          if (key < item->next->key)
          {
            new_item->next = item->next;
            item->next = new_item;
            break;
          }
        }
      } while ((item = item->next));
    }
  }
  target->size++;
  return new_item;
}

/**
   Remove and return tail data of list
   This is expensive: singly linked list.
*/
void*
ilist_pop(struct ilist* target)
{
  // NOTE_F;

  void* data;
  if (target->size == 0)
    return NULL;
  if (target->size == 1)
  {
    data = target->head->data;
    free(target->head);
    target->head = NULL;
    target->tail = NULL;
    target->size = 0;
    return data;
  }

  struct ilist_item* item;
  for (item = target->head; item->next->next;
       item = item->next);
  data = item->next->data;
  free(item->next);
  item->next = NULL;
  target->tail = item;
  target->size--;
  return data;
}

/**
   Remove and return head of list.
 */
void*
ilist_poll(struct ilist* target)
{
  void* data;
  if (target->size == 0)
    return NULL;
  if (target->size == 1)
  {
    data = target->head->data;
    free(target->head);
    target->head = NULL;
    target->tail = NULL;
    target->size = 0;
    return data;
  }

  struct ilist_item* delendum = target->head;
  data = target->head->data;
  target->head = target->head->next;
  free(delendum);
  target->size--;
  return data;
}

/**
   Return data item i or NULL if i is out of bounds.
*/
void*
ilist_get(struct ilist* target, int i)
{
  struct ilist_item* item;
  int j = 0;
  for (item = target->head; item; item = item->next, j++)
    if (i == j)
      return (item->data);
  return NULL;
}

/**
  @return The data or NULL if not found.
*/
void*
ilist_search(struct ilist* target, int key)
{
 struct ilist_item* item;
  for (item = target->head; item; item = item->next)
    if (key == item->key)
      return (item->data);
  return NULL;
}

/**
   Removes the ilist_item from the list.
   frees the ilist_item.
   @return The data item or NULL if not found.
*/
void*
ilist_remove(struct ilist* target, int key)
{
  struct ilist_item* item;
  void* data;
  if (target->size == 0)
    return false;
  if (target->head->key == key)
  {
    item = target->head;
    target->head = item->next;
    data = item->data;
    free(item);
    target->size--;
    return data;
  }
  for (item = target->head; item->next; item = item->next)
    if (item->next->key == key)
    {
      struct ilist_item* delendum = item->next;
      data = delendum->data;
      if (item->next == target->tail)
        target->tail = item;
      item->next = item->next->next;
      free(delendum);
      target->size--;
      return data;
    }
  return NULL;
}

/**
   Free this list but not its data.
*/
void
ilist_free(struct ilist* target)
{
  struct ilist_item* item = target->head;
  while (item)
  {
    struct ilist_item* next = item->next;
    free(item);
    item = next;
  }
  free(target);
}

/**
   Free this list and its data.
*/
void
ilist_destroy(struct ilist* target)
{
  // NOTE_F;
  struct ilist_item* item = target->head;
  while (item)
  {
    struct ilist_item* next = item->next;
    free(item->data);
    // free(item);
    item = next;
  }
  free(target);
  // DONE;
}

/**
   @param format Specifies the output format for the data items.
 */
void
ilist_printf(char* format, struct ilist* target)
{
  struct ilist_item* item;
  printf("[");
  for (item = target->head;
       item; item = item->next)
  {
    printf("(%i,", item->key);
    if (strcmp(format, "%s") == 0)
      printf(format, item->data);
    else if (strcmp(format, "%i") == 0)
      printf(format, *((int*) (item->data)));
    printf(")");
    if (item->next)
      printf(",");
  }
  printf("]\n");
}

/**
   @param format Specifies the output format for the data items.
 */
void
ilist_fprintf(FILE* file, char* format, struct ilist* target)
{
  struct ilist_item* item;
  fprintf(file, "[");
  for (item = target->head;
       item; item = item->next)
  {
    fprintf(file, "(%i,", item->key);
    if (strcmp(format, "%s") == 0)
      fprintf(file, format, item->data);
    else if (strcmp(format, "%i") == 0)
      fprintf(file, format, *((int*) (item->data)));
    fprintf(file, ")");
    if (item->next)
      fprintf(file, ",");
  }
  fprintf(file, "]\n");
}

/**
   @param f Specifies the output format for the data items.
 */
void
ilist_fdump(FILE* file, char* (f)(void*), struct ilist* target)
{
  struct ilist_item* item;
  fprintf(file, "[");
  for (item = target->head;
       item; item = item->next)
  {
    fprintf(file, "(%i,", item->key);
    fprintf(file, "%s", f(item->data));
    fprintf(file, ")");
    if (item->next)
      fprintf(file, ",");
  }
  fprintf(file, "]\n");
}

/**
   Just dump the int keys.
 */
void
ilist_dumpkeys(struct ilist* target)
{
  struct ilist_item* item;
  printf("KEYS: [");
  for (item = target->head;
       item; item = item->next)
  {
    printf("(%i)", item->key);
    if (item->next)
      printf(",");
  }
  printf("]\n");
}

/**
   Just dump the int keys.
 */
void
ilist_fdumpkeys(FILE* file, struct ilist* target)
{
  struct ilist_item* item;
  fprintf(file, "KEYS: [");
  for (item = target->head;
       item; item = item->next)
  {
    fprintf(file, "(%i)", item->key);
    if (item->next)
      fprintf(file, ",");
  }
  fprintf(file, "]\n");
}

/**
   Dump the int keys in hex.
 */
void
ilist_xdumpkeys(struct ilist* target)
{
  struct ilist_item* item;
  printf("[");
  for (item = target->head;
       item; item = item->next)
  {
    printf("(%x)", item->key);
    if (item->next)
      printf(",");
  }
  printf("]\n");
}

/**
   Just dump the data pointers.
   @return Allocated memory: 10 * target->size.
 */
char*
ilist_serialize_ptrs(struct ilist* target)
{
  char* result = malloc(30*target->size);
  struct ilist_item* item;
  char* p = result;
  p += sprintf(p, "PTRS: [");
  for (item = target->head;
       item; item = item->next)
  {
    p += sprintf(p, "%p", item->data);
    if (item->next)
      p += sprintf(p, " ");
  }
  sprintf(p, "]\n");
  return result;
}

static char*
append_pair(char* ptr, struct ilist_item* item, char* s)
{
  ptr += sprintf(ptr, "(%i,", item->key);
  ptr += sprintf(ptr, "%s)", s);

  if (item->next)
    ptr += sprintf(ptr, ",");
  return ptr;
}

/**
   @param f The output function for the data.
*/
void
ilist_dump(char* (*f)(void*), struct ilist* target)
{
  struct ilist_item* item;
  printf("[");
  for (item = target->head;
       item; item = item->next)
  {
    printf("(%i,", item->key);
    printf("%s", f(item->data));
    printf(")");
    if (item->next)
      printf(",");
  }
  printf("] \n");
}

/** Dump ilist to string a la snprintf()
    size must be greater than 2.
    format specifies the output format for the data items
    returns int greater than size if size limits are exceeded
            indicating result is garbage
 */
int ilist_snprintf(char* str, size_t size,
                   char* format, struct ilist* target)
{
  int               error = size+1;
  char*             ptr   = str;
  struct ilist_item* item;

  if (size <= 2)
    return error;

  ptr += sprintf(ptr, "[");

  char* s = (char*) malloc(sizeof(char)*ILIST_MAX_DATUM);

  for (item = target->head; item && ptr-str < size;
       item = item->next)

  {
    int   r = snprintf(s, ILIST_MAX_DATUM, format, item->data);
    if (r > ILIST_MAX_DATUM)
      return size+1;
    if ((ptr-str) + 10 + r + 4 < size)
      ptr = append_pair(ptr, item, s);
    else
      return error;
  }
  ptr += sprintf(ptr, "]");

  free(s);
  return (ptr-str);
}

/** Dump ilist to string a la snprintf()
    size must be greater than 2.
    format specifies the output format for the data items
    returns int greater than size if size limits are exceeded
            indicating result is garbage
 */
int ilist_marshal(char* str, size_t size,
                  char* (f)(void*), struct ilist* target)
{
  int               error = size+1;
  char*             ptr   = str;
  struct ilist_item* item;

  if (size <= 2)
    return error;

  ptr += sprintf(ptr, "[");

  for (item = target->head; item && ptr-str < size;
       item = item->next)
  {
    char* s = f(item->data);
    int   r = sprintf(s, "%s", s);
    if (r > ILIST_MAX_DATUM)
      return size+1;
    if ((ptr-str) + 10 + r + 4 < size)
      ptr = append_pair(ptr, item, s);
    else
      return error;
  }
  ptr += sprintf(ptr, "]");

  return (ptr-str);
}

#ifdef DEBUG_ILIST

#endif
