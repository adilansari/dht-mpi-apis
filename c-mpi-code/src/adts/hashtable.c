
#include "hashtable.h"

/**
 * derived from an algorithm found in Aho, Sethi and Ullman's
 * {Compilers: Principles, Techniques and Tools}, published by Addison-Wesley.
 * This algorithm comes from P.J. Weinberger's C compiler.
 */
int
quickhash_string_hash(char* key, int table_size)
{
  uint32_t h=0,g=0;

  if (! key)
    return -1;

  while (*key)
  {
    h = (h << 4) + *key++;
    if ((g = (h & 0xF0UL)))
    {
      h ^= g >> 24;
      h ^= g;
    }
  }

  return (int)(h & ((uint64_t)(table_size - 1)));
}

int
hash_string(char* data, int table_size)
{
  // dump_stack();
  int sha1  = SHA1_mod(data);
  int index = sha1 % table_size;

  // printf("hash_string %s -> %i \n", data, index);

  return index;
}

/**
   Compress SHA-1 down to a smaller address space,
   namely, 4 bytes, and make positive.
*/
int
SHA1_mod(char* data)
{
  static unsigned char output[SHA_DIGEST_LENGTH];

  SHA1((unsigned char*) data, strlen(data), output);

  int result;
  memcpy(&result, output+16, 4);

  // printf("KEY: %s %X \n", data, result);

  return abs(result);
}

struct hashtable*
hashtable_create(int capacity)
{
  // NOTE_F;

  int i = 0;
  struct hashtable *new_table = NULL;

  new_table =
    (struct hashtable*) malloc(sizeof(struct hashtable));
  if (! new_table)
    return (NULL);

  new_table->size     = 0;
  new_table->capacity = capacity;

  new_table->array =
    (struct klist**) malloc(sizeof(struct klist*) * capacity);
  if (!new_table->array)
  {
    free(new_table);
    return (NULL);
  }

  for (i = 0; i < capacity; i++)
  {
    struct klist* new_klist = klist_create();
    if (! new_klist)
      return NULL;
    new_table->array[i] = new_klist;
  }

  return (new_table);
}

void
hashtable_free(struct hashtable* target)
{
  // NOTE_F;

  for (int i = 0; i < target->capacity; i++)
    klist_free(target->array[i]);

  free(target->array);
  free(target);

  return;
}

void
hashtable_destroy(struct hashtable* target)
{
  for (int i = 0; i < target->capacity; i++)
    klist_destroy(target->array[i]);

  free(target->array);
  free(target);
}

bool
hashtable_add(struct hashtable *table, char* key, void* data)
{
  // NOTE_F;

  int index = hash_string(key, table->capacity);

  struct klist_item* new_item =
    klist_add(table->array[index], key, data);

  if (! new_item)
  {
    printf("No new item! \n");
    return false;
  }

  table->size++;

  return true;
}

/**
   @return A pointer to the matching data or NULL if not found.
*/
void*
hashtable_search(struct hashtable* table, char* key)
{
  struct klist_item* item = NULL;
  int index = hash_string(key, table->capacity);

  for (item = table->array[index]->head; item;
       item = item->next)
    if (strcmp(key, item->key) == 0)
      return item->data;

  return NULL;
}

/** format specifies the output format for the data items
 */
void
hashtable_dump(char* format, struct hashtable* target)
{
  char s[200];
  int i;
  printf("{\n");
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size > 0)
    {
      klist_tostring(s, 200, "%s", target->array[i]);
      printf("%i: %s \n", i, s);
    }
  printf("}\n");
}

void
hashtable_dumpkeys(struct hashtable* target)
{
  int i;
  printf("{\n");
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size)
    {
      printf("%i:", i);
      klist_dumpkeys(target->array[i]);
    }
  printf("}\n");
}

/** Dump klist to string a la snprintf()
    size must be greater than 2.
    format specifies the output format for the data items
    internally allocates O(size) memory
    returns int greater than size if size limits are exceeded
            indicating result is garbage
*/
int hashtable_tostring(char* str, size_t size,
                       char* format, struct hashtable* target)
{
  int   error = size+1;
  char* ptr   = str;
  int i;
  ptr += sprintf(str, "{\n");

  char* s = (char*) malloc(sizeof(char) * size);

  for (i = 0; i < target->capacity; i++)
  {
    int r = klist_tostring(s, size, format, target->array[i]);
    if ((ptr-str) + r + 2 < size)
      ptr += sprintf(ptr, "%s\n", s);
    else
      return error;
  }
  sprintf(ptr, "}\n");

  free(s);
  return (ptr-str);
}

/**
   Debugging only.
*/

#ifdef DEBUG_HASHTABLE
int
main()
{
  struct hashtable* table = hashtable_create(11);

  hashtable_add(table, "key1", "val1");

  hashtable_dump("%s", table);

}
#endif
