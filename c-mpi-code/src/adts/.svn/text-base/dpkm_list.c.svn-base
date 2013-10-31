
#include "dpkm_list.h"

struct dpkm_list*
dpkm_list_create(int limit)
{
  // SHOW_FI(limit);
  struct dpkm_list* new_dpkm_list = malloc(sizeof(struct dpkm_list));
  if (! new_dpkm_list)
    return NULL;
  new_dpkm_list->head  = NULL;
  new_dpkm_list->tail  = NULL;
  new_dpkm_list->size  = 0;
  new_dpkm_list->limit = limit;
  return new_dpkm_list;
}

/**
   Adds item to tail.
   @return If list size exceeds limit,
   return item to flush, else NULL.
*/
struct dpkm_list_item*
dpkm_list_add(struct dpkm_list* target, char* key, void* data, int length)
{
  struct dpkm_list_item* result = NULL;

  struct dpkm_list_item* new_item = malloc(sizeof(struct dpkm_list_item));
  if (! new_item)
    return NULL;

  new_item->key    = key;
  new_item->data   = data;
  new_item->next   = NULL;
  new_item->length = length;

  if (target->size == 0)
  {
    target->head = new_item;
    target->tail = new_item;
    new_item->prev = NULL;
  }
  else
  {
    new_item->prev     = target->tail;
    target->tail->next = new_item;
  }

  target->tail = new_item;
  target->size++;

  if (target->size > target->limit)
  {
    result = dpkm_list_poll(target);
  }

  return result;
}

/**
   Move this item to the tail of the list.
*/
void
dpkm_list_pushback(struct dpkm_list* target,
                   struct dpkm_list_item* item)
{
  // NOTE_F;
  /*
  printf("item: %s \n", item->key);
  if (item->next == NULL)
    printf("NULL\n");
  else
    printf("not NULL\n");
  */
  dpkm_list_excise(target, item);
  dpkm_list_append(target, item);
}


struct dpkm_list_item*
dpkm_list_search(struct dpkm_list* target, char* key)
{
  struct dpkm_list_item* item;
  for (item = target->head; item; item = item->next)
    if (strcmp(key, item->key) == 0)
      return item;

  return NULL;
}


/**
   Add this data if dpkm_list_inspect does not find it.
*/
/*
struct dpkm_list_item*
dpkm_list_add_one(struct dpkm_list* target, void* data, int n)
{
  if (! dpkm_list_inspect(target, data, n))
    return dpkm_list_add(target, data);
  return NULL;
}
*/

/**
   Add this pre-formed dpkm_list_item to target.
   Convenience: sets item->next to NULL.
   @return The added item.
*/
struct dpkm_list_item*
dpkm_list_append(struct dpkm_list* target,
                 struct dpkm_list_item* item)
{
  if (target->size == 0)
  {
    target->head = item;
  }
  else
  {
    item->prev = target->tail;
    target->tail->next = item;
  }
  target->tail       = item;

  item->next         = NULL;
  target->size++;
  return item;
}

/**
   Cut the item out of target dpkm_list
*/
void
dpkm_list_excise(struct dpkm_list* target,
                 struct dpkm_list_item* item)
{
  struct dpkm_list_item* prev = item->prev;
  struct dpkm_list_item* next = item->next;

  // NOTE_F;

  // printf("EXCISE %i \n", target->size);
  // dpkm_list_dump("%i", target);

  if (prev == NULL && next == NULL)
  {
    assert(target->size == 1);
    target->head = NULL;
    target->tail = NULL;
  }
  else if (prev == NULL)
  {
    next->prev = NULL;
    target->head = next;
  }
  else if (next == NULL)
  {
    prev->next = NULL;
    target->tail = prev;
  }
  else
  {
    prev->next = next;
    next->prev = prev;
  }

  target->size--;
}

/**
   Remove and return list tail.
 */
  /*
void*
dpkm_list_pop(struct dpkm_list* target)
{
  void* data;
  if (target->size == 0)
    return NULL;
  if (target->size == 1)
  {
    data = target->head->data;
    // free(target->head);
    target->head = NULL;
    target->tail = NULL;
    target->size = 0;
    return data;
  }

  struct dpkm_list_item* item;
  for (item = target->head; item->next->next;
       item = item->next);
  data = item->next->data;
  // free(item->next);
  item->next = NULL;
  target->tail = item;
  target->size--;
  return data;
}
  */

/**
   Return list head data.
*/
void*
dpkm_list_peek(struct dpkm_list* target)
{
  if (target->size == 0)
    return NULL;
  return target->head->data;
}

/**
   Remove and return list head.
 */
struct dpkm_list_item*
dpkm_list_poll(struct dpkm_list* target)
{
  struct dpkm_list_item* result;
  struct dpkm_list_item* new_head;

  // NOTE_F;
  // SHOW_I(target->size);

  if (target->size == 0)
    return NULL;

  if (target->size == 1)
  {
    result = target->head;
    target->head = NULL;
    target->tail = NULL;
    target->size = 0;
    return result;
  }

  // SHOW_S(target->head->key);

  new_head = target->head->next;
  result   = target->head;
  target->head   = new_head;
  new_head->prev = NULL;

  target->size--;

  return result;
}

/*
void*
dpkm_list_random(struct dpkm_list* target)
{
  printf("%s %i \n", "dpkm_list size: ", target->size);

  if (target->size == 0)
    return NULL;

  int p = rand() % target->size;
  struct dpkm_list_item* item = target->head;
  for (int i = 0; i < p; i++)
    item = item->next;

  return item->data;
}
*/

 /*
struct dpkm_list_item*
dpkm_list_ordered_insert(struct dpkm_list* target,
                    int (*cmp)(void*,void*), void* data)
{
  // printf("ordered insert\n");
  struct dpkm_list_item* new_item = malloc(sizeof(struct dpkm_list_item));
  if (! new_item)
    return NULL;
  new_item->data = data;
  new_item->next = NULL;

  if (target->size == 0)
  {
    target->head = new_item;
    target->tail = new_item;
  }
  else
  {
    struct dpkm_list_item* item = target->head;
    // Are we the new head?
    if (cmp(data, item->data) == -1)
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
          if (cmp(data, item->next->data) == -1)
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
 */

/**
   Untested.
 */
  /*
struct dpkm_list_item*
dpkm_list_ordered_insert_unique(struct dpkm_list* target, int (*cmp)(void*,void*),
                           void* data)
{
  struct dpkm_list_item* new_item = malloc(sizeof(struct dpkm_list_item));
  if (! new_item)
    return NULL;
  new_item->data = data;
  new_item->next = NULL;

  if (target->size == 0)
  {
    target->head = new_item;
    target->tail = new_item;
  }
  else
  {
    struct dpkm_list_item* item = target->head;
    // Are we the new head?
    if (cmp(data, item->data) == -1)
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
          int c = cmp(data, item->next->data);
          if (c == 0)
          {
          // free(new_item);
            return NULL;
          }
          if (c == -1)
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
  */

   /*
struct dpkm_list_item*
dpkm_list_add_unique(struct dpkm_list* target,
                int (*cmp)(void*,void*), void* data)
{
  if (! dpkm_list_contains(target, data, cmp))
    return dpkm_list_add(target, data);
  return NULL;
}
   */

/**
 */
    /*
bool
dpkm_list_contains(struct dpkm_list* target, int (*cmp)(void*,void*),
              void* data)
{
  for (struct dpkm_list_item* item = target->head;
       item; item = item->next)
    if (cmp(item->data, data) == 0)
      return true;
  return false;
}
*/

/**
   Compare data pointer addresses for match.
   @return An equal data pointer or NULL if not found.
*/
     /*
void*
dpkm_list_search(struct dpkm_list* target, void* data)
{
  for (struct dpkm_list_item* item = target->head;
       item; item = item->next)
    if (item->data == data)
      return data;
  return NULL;
}
     */

/**
   Compare data contents for match.
   @return A pointer to an equivalent object or NULL if not found.
 */
      /*
void*
dpkm_list_inspect(struct dpkm_list* target, void* data, size_t n)
{
  for (struct dpkm_list_item* item = target->head;
       item; item = item->next)
    if (memcmp(item->data, data, n) == 0)
      return item->data;
  return NULL;
}
*/

/**
   Empty the dpkm_list.
*/
       /*
void
dpkm_list_clear(struct dpkm_list* target)
{
  struct dpkm_list_item* item = target->head;

  while (item)
  {
    struct dpkm_list_item* next = item->next;
    // free(item);
    item = next;
  }
  target->head = NULL;
  target->tail = NULL;
  target->size = 0;
}
       */

/**
   Empty the dpkm_list and free the data.
*/
        /*
void
dpkm_list_clobber(struct dpkm_list* target)
{
  struct dpkm_list_item* item = target->head;

  while (item)
  {
    struct dpkm_list_item* next = item->next;
    free(item->data);
    free(item);
    item = next;
  }
  target->head = NULL;
  target->tail = NULL;
}
        */

/**
   Removes only one item that points to given data.
   Does not free the item data.
   @return True iff the data pointer was matched
            and the item was freed.
*/
bool
dpkm_list_remove(struct dpkm_list* target, void* data)
{
  if (target->size == 0)
    return false;

  struct dpkm_list_item* item = target->head;

  if (data == item->data)
  {
    struct dpkm_list_item* next = item->next;
    free(item);
    target->head = next;
    if (target->tail == next)
      target->tail = NULL;
    target->size--;
    return true;
  }

  while (item->next)
  {
    // Are we removing the item after this item?
    if (data == item->next->data)
    {
      struct dpkm_list_item* nextnext = item->next->next;
      if (target->tail == item->next)
        target->tail = nextnext;
      free(item->next);
      item->next = nextnext;
      target->size--;
      return true;
    }
    item = item->next;
  }
  return false;
}

/**
   Return all elements from the dpkm_list where cmp(data,arg) == 0.
*/
/*
struct dpkm_list*
dpkm_list_select(struct dpkm_list* target,
            int (*cmp)(void*,void*), void* arg)
{
  assert(target != NULL);

  struct dpkm_list* result = dpkm_list_create();

  for (struct dpkm_list_item* item = target->head;
       item; item = item->next)
  {
    if (cmp(item->data, arg) == 0)
      dpkm_list_add(result, item->data);
  }

  return result;
}
*/

/**
   Remove the elements from the dpkm_list where cmp(data,arg) == 0.
   @return true if one or more items were deleted.
*/
/*
bool
dpkm_list_remove_where(struct dpkm_list* target,
                  int (*cmp)(void*,void*), void* arg)
{
  bool result = false;

  if (target->size == 0)
    return false;

  int old_size = target->size;

  // Establish next good item in dpkm_list...
  struct dpkm_list_item* good = NULL;
  for (struct dpkm_list_item* item = target->head;
       item; item = item->next)
  {
    if (cmp(item->data, arg) != 0)
    {
      good = item;
      break;
    }
  }

  if (! good)
    // Dpkm_List should be empty
  {
    if (target->size > 0)
      result = true;
    dpkm_list_clear(target);
    return result;
  }

  // Establish correct head...
  struct dpkm_list_item* head = target->head;
  while (head && head != good)
  {
    struct dpkm_list_item* next = head->next;
    printf("free: %i \n", *(int*) head->data);
    free(head);
    target->size--;
    head = next;
  }
  target->head = good;

  // Now current points to the first valid item in the dpkm_list.
  struct dpkm_list_item* current = target->head;
  while (good != NULL)
  {
    // Move to a good item or NULL...
    struct dpkm_list_item* item = good->next;
    good = NULL;
    while (item)
    {
      if (cmp(item->data, arg) != 0)
      {
        good = item;
        break;
      }
      item = item->next;
    }

    if (good == NULL)
      // No more good items were found
    {
      target->tail = current;
    }

    // Free items between current and good:

    struct dpkm_list_item* link = current;
    current = current->next;
    while (current != good)
    {
      struct dpkm_list_item* next = current->next;
      free(current);
      target->size--;
      current = next;
    }
    link->next = good;
  }

  if (target->size != old_size)
    return true;
  return false;
}
*/

/**
   Remove and return all elements from the dpkm_list where
   cmp(data,arg) == 0.
   @return true if one or more items were deleted.
*/
/*
struct dpkm_list*
dpkm_list_pop_where(struct dpkm_list* target,
                  int (*cmp)(void*,void*), void* arg)
{
  struct dpkm_list* result = dpkm_list_create();

  if (target->size == 0)
    return result;

  // Establish next good item in dpkm_list...
  struct dpkm_list_item* good = NULL;
  for (struct dpkm_list_item* item = target->head;
       item; item = item->next)
  {
    if (cmp(item->data, arg) != 0)
    {
      good = item;
      break;
    }
  }

  if (! good)
    // All elements should be moved
  {
    dpkm_list_transplant(result, target);
    return result;
  }

  // Establish correct head...
  struct dpkm_list_item* head = target->head;
  while (head && head != good)
  {
    struct dpkm_list_item* next = head->next;
    printf("free: %i \n", *(int*) head->data);
    dpkm_list_append(result, head);
    target->size--;
    head = next;
  }
  target->head = good;

  // Now current points to the first valid item in the dpkm_list.
  struct dpkm_list_item* current = target->head;
  while (good != NULL)
  {
    // Move to a good item or NULL...
    struct dpkm_list_item* item = good->next;
    good = NULL;
    while (item)
    {
      if (cmp(item->data, arg) != 0)
      {
        good = item;
        break;
      }
      item = item->next;
    }

    if (good == NULL)
      // No more good items were found
      target->tail = current;

    if (good != NULL)
      printf("good: %i \n", *(int*) good->data);

    // Free items between current and good:

    struct dpkm_list_item* link = current;
    current = current->next;
    while (current != good)
    {
      struct dpkm_list_item* next = current->next;
      dpkm_list_append(result, current);
      target->size--;
      current = next;
    }
    link->next = good;
  }

  return result;
}
*/

/**
   Attach copy of tail to target.
*/
void
dpkm_list_attach(struct dpkm_list* target, struct dpkm_list* segment);

/**
   Moves all items from tail into target structure.
*/
/*
void
dpkm_list_transplant(struct dpkm_list* target, struct dpkm_list* segment)
{
  if (target->size == 0)
  {
    target->head = segment->head;
  }
  target->tail->next = segment->head;
  target->size += segment->size;
  target->tail  = segment->tail;

  segment->head = NULL;
  segment->tail = NULL;
  segment->size = 0;
}
*/

/**
   Does not free the item data.
   @return True iff the data content was matched by memcmp
            and the item was freed.
*/
/*
bool
dpkm_list_erase(struct dpkm_list* target, void* data, size_t n)
{
  struct dpkm_list_item* item = target->head;
  // Are we removing the head?
  if (memcmp(data, item->data, n) == 0)
  {
    struct dpkm_list_item* next = item->next;
    free(item);
    target->head = next;
    if (target->tail == next)
      target->tail = NULL;
    target->size--;
    return true;
  }
  do
  {
    // Are we removing the item after this item?
    if (memcmp(data, item->next->data, n) == 0)
    {
      struct dpkm_list_item* nextnext = item->next->next;
      if (target->tail == item->next)
        target->tail = nextnext;
      free(item->next);
      item->next = nextnext;
      target->size--;
      return true;
    }
  } while ((item = item->next));
  return false;
}
*/

/**
   Function specifies the output format for the data items
   Does not free return of f.
 */
/*
void
dpkm_list_output(char* (*f)(void*), struct dpkm_list* target)
{
  struct dpkm_list_item* item;

  printf("[");
  for (item = target->head; item; item = item->next)
  {
    printf("%s", f(item->data));
    if (item->next)
      printf(",");
  }
  printf("]\n");
}
*/

/**
   @param format Specifies the output format for the data items.
 */
void
dpkm_list_printf(char* format, struct dpkm_list* target)
{
  struct dpkm_list_item* item;

  printf("[");
  for (item = target->head; item; item = item->next)
  {
    printf("(%s,", item->key);
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
dpkm_list_fprintf(FILE* file, char* format, struct dpkm_list* target)
{
  struct dpkm_list_item* item;

  fprintf(file, "[");
  for (item = target->head; item; item = item->next)
  {
    fprintf(file, "(%s,", item->key);
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
   Print list using printdata() for values.
 */
void
dpkm_list_fprintdata(FILE* file, struct dpkm_list* target)
{
  struct dpkm_list_item* item;

  fprintf(file, "[");
  for (item = target->head; item; item = item->next)
  {
    fprintf(file, "(%s@%i,", item->key, item->length);
    printdata("", item->data, item->length);
    printf(")");
    if (item->next)
      printf(",");
  }
  printf("]\n");
}

void
dpkm_list_printdata(struct dpkm_list* target)
{
  dpkm_list_fprintdata(stdout, target);
}

/**
   Free this dpkm_list but not its data.
*/
void
dpkm_list_free(struct dpkm_list* target)
{
  struct dpkm_list_item* item = target->head;
  while (item)
  {
    struct dpkm_list_item* next = item->next;
    free(item);
    item = next;
  }
  free(target);
}

/**
   Free this dpkm_list and its data but not its
   extra pointers.
*/
void
dpkm_list_destroy(struct dpkm_list* target)
{
  struct dpkm_list_item* item = target->head;
  while (item)
  {
    struct dpkm_list_item* next = item->next;
    free(item->key);
    if (item->data)
      free(item->data);
    free(item);
    item = next;
  }
  free(target);
}

/*
int
int_cmp(void* i1, void* i2)
{
  int j1 = *(int*) i1;
  int j2 = *(int*) i2;

  if (j1 > j2)
    return 1;
  else if (j1 < j2)
    return -1;
  else
    return 0;
}
*/

/**
   Returns 0 iff i1 is divisible by i2.
*/
/*
int
divides_cmp(void* i1, void* i2)
{
  int j1 = *(int*) i1;
  int j2 = *(int*) i2;

  return (j1 % j2);
}
*/

/**
   Assumes item data is character.
*/
char* dpkm_pair_tostring(char* ptr, char* format,
                         struct dpkm_list_item* item)
{
  ptr += sprintf(ptr, "(%s,", item->key);

  if (strcmp(format, "%s") == 0)
    ptr += sprintf(ptr, format, (char*) item->data);
  else if (strcmp(format, "%i") == 0)
    ptr += sprintf(ptr, format, *(int*) item->data);
  ptr += sprintf(ptr, ")");

  if (item->next)
    ptr += sprintf(ptr, ",");
  return ptr;
}


/** Dump dpkm_list to string a la snprintf()
    size must be greater than 2.
    format specifies the output format for the data items
    returns int greater than size if size limits are exceeded
            indicating result is garbage
*/
int dpkm_list_tostring(char* str, size_t size,
                       char* format, struct dpkm_list* target)
{
  int               error = size+1;
  char*             ptr   = str;
  struct dpkm_list_item* item;

  if (size <= 2)
    return error;

  ptr += sprintf(ptr, "[");

  for (item = target->head; item && (ptr-str < size);
       item = item->next)
  {
    if ((ptr-str) + strlen(item->key) + strlen(item->data) + 4 < size)
      ptr = dpkm_pair_tostring(ptr, format, item);
    else
      return error;
  }
  sprintf(ptr, "]");

  return (ptr-str);
}


