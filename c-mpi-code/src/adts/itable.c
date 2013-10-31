
#include "itable.h"

int
hash_int(int key, int table_size)
{
  return (key % table_size);
}

struct itable*
itable_create(int capacity)
{
  int i = 0;
  struct itable *new_table = NULL;

  new_table =
    (struct itable*) malloc(sizeof(struct itable));
  if (! new_table)
    return (NULL);

  new_table->size     = 0;
  new_table->capacity = capacity;

  new_table->array =
    (struct ilist**) malloc(sizeof(struct ilist*) * capacity);
  if (!new_table->array)
  {
    free(new_table);
    return (NULL);
  }

  for (i = 0; i < capacity; i++)
  {
    struct ilist* new_ilist = ilist_create();
    if (! new_ilist)
      return NULL;
    new_table->array[i] = new_ilist;
  }

  return new_table;
}

/*
void
itable_finalize(struct itable *old_table)
{
  // DOES NOT free ILISTS!
  free(old_table->array);
  free(old_table);
  return;
}
*/

/**
   Add key/data pair to table.
   If key exists, do nothing and return false
*/
bool
itable_add(struct itable* table, int key, void* data)
{
  int index = hash_int(key, table->capacity);

  bool result = ilist_add(table->array[index], key, data);

  if (result)
    table->size++;

  return result;
}

/**
  @return The data or NULL if not found.
*/
void*
itable_search(struct itable* table, int key)
{
  int index = hash_int(key, table->capacity);
  return ilist_search(table->array[index], key);
}

void*
itable_remove(struct itable* table, int key)
{
  int index = hash_int(key, table->capacity);
  void* result = ilist_remove(table->array[index], key);
  if (result)
    table->size--;
  return result;
}

/**
   Should only be called on an empty table
*/
void
itable_free(struct itable* target)
{
  int i;

  // NOTE_F;
  // SHOW_FI(target->size);

  for (i = 0; i < target->capacity; i++)
  {
    ilist_free(target->array[i]);
  }
  free(target->array);
  free(target);
}

void
itable_destroy(struct itable* target)
{
  int i;

  // NOTE_F;
  // SHOW_FI(target->size);

  for (i = 0; i < target->capacity; i++)
  {
    ilist_destroy(target->array[i]);
  }
  free(target->array);
  free(target);

  // DONE;
}

/**
   @param format specifies the output format for the data items
 */
void
itable_dump(char* format, struct itable* target)
{
  int i;
  char s[200];
  printf("{\n");
  for (i = 0; i < target->capacity; i++)
  {
    if (target->array[i]->size > 0)
    {
      ilist_snprintf(s, 200, "%s", target->array[i]);
      printf("%i: %s \n", i, s);
    }
  }
  printf("}\n");
}

/** Dump ilist to string a la snprintf()
    size must be greater than 2.
    format specifies the output format for the data items
    internally allocates O(size) memory
    returns int greater than size if size limits are exceeded
            indicating result is garbage
*/
int itable_tostring(char* str, size_t size,
                       char* format, struct itable* target)
{
  int   error = size+1;
  char* ptr   = str;
  int i;
  ptr += sprintf(str, "{\n");

  char* s = (char*) malloc(sizeof(char) * size);

  for (i = 0; i < target->size; i++)
  {
    int r = ilist_snprintf(s, size, format, target->array[i]);
    if ((ptr-str) + r + 2 < size)
      ptr += sprintf(ptr, "%s\n", s);
    else
      return error;
  }
  sprintf(ptr, "}\n");

  free(s);
  return (ptr-str);
}

#ifdef DEBUG_ITABLE

int
main()
{
  char s[200];
  struct itable* table = itable_create(30);

  itable_add(table, 30, "hello30");
  itable_add(table, 22, "hello22");
  itable_add(table, 21, "hello21");
  itable_add(table, 51, "hello51");

  // itable_tostring(s, 200, "%s", table);
  itable_dump("%s", table);

  itable_remove(table, 22);

  itable_dump("%s", table);
}

#endif
