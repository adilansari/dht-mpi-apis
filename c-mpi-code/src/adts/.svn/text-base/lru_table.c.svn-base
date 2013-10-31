
#include "lru_table.h"

static int
max(int i1, int i2)
{
  return (i1>i2) ? i1 : i2;
}

/*
static int
min(int i1, int i2)
{
  return (i1<i2) ? i1 : i2;
}
*/

struct lru_table*
lru_table_create(int capacity, int limit)
{
  // SHOW_FII(capacity, limit);

  int i = 0;
  struct lru_table *new_table = NULL;

  new_table =
    (struct lru_table*) malloc(sizeof(struct lru_table));
  if (! new_table)
    return (NULL);

  new_table->size     = 0;
  new_table->capacity = capacity;
  new_table->limit    = limit;

  new_table->lru = dpkm_list_create(limit);

  new_table->array =
    (struct dpkm_list**) malloc(capacity * sizeof(struct dpkm_list*));
  if (!new_table->array)
  {
    free(new_table);
    return NULL;
  }

  for (i = 0; i < capacity; i++)
  {
    // These lists do not enforce a limit!
    struct dpkm_list* new_dpkm_list = dpkm_list_create(limit+3);
    if (! new_dpkm_list)
      return NULL;
    new_table->array[i] = new_dpkm_list;
  }

  return new_table;
}

/**
   Does not free the data.
*/
void
lru_table_free(struct lru_table* target)
{
  NOTE_F;
  int i;
  for (i = 0; i < target->capacity; i++)
    dpkm_list_free(target->array[i]);
  free(target->array);
  dpkm_list_free(target->lru);
  free(target);
}

/**
   Frees structure and the data.
*/
void
lru_table_destroy(struct lru_table* target)
{
  NOTE_F;
  int i;
  for (i = 0; i < target->capacity; i++)
    dpkm_list_destroy(target->array[i]);
  free(target->array);
  // Do not free the keys or data in this list- they are referred
  //    to by the array entries
  dpkm_list_free(target->lru);
  free(target);
}


/**
   Adds the key/data pair to the table.

   Adds new item to end of table list and lru list.
   Sets those two item ptrs to each other.

   @param key May not be NULL
   @param data

   @return NULL if the table is not overfull or a
           struct keyvalue* representing the key/data pair
           that was LRU and selected as a victim.
 */
struct keyvalue*
lru_table_add(struct lru_table* table, char* key,
              void* data, int length)
{
  NOTE_FS(key);
  assert(key);

  struct keyvalue* result = NULL;

  int index = hash_string(key, table->capacity);

  // printf("%s -> %i \n", key, index);

  dpkm_list_add(table->array[index], key, data, length);

  struct dpkm_list_item* victim =
    dpkm_list_add(table->lru, key, data, length);

  table->array[index]->tail->ptr = table->lru->tail;
  table->lru->tail->ptr          = table->array[index]->tail;

  if (victim == NULL)
  {
    table->size++;
  }
  else
  {
    int victim_index = hash_string(victim->key, table->capacity);
    dpkm_list_excise(table->array[victim_index], victim->ptr);
    result = (struct keyvalue*) malloc(sizeof(struct keyvalue));
    result->key    = victim->key;
    result->data   = victim->data;
    result->length = victim->length;
    free(victim->ptr);
    free(victim);
  }

  return result;
}

/**
   Search the table and touch the key/data pair
   to prevent vicitimization.

   @return Allocated keyvalue with pointers to key/data in table.
*/
struct keyvalue*
lru_table_search(struct lru_table* table, char* key)
{
  SHOW_FS(key);

  struct dpkm_list_item* item = NULL;
  int index = hash_string(key, table->capacity);
  struct keyvalue* result;

  item = dpkm_list_search(table->array[index], key);
  if (item)
  {
    dpkm_list_pushback(table->lru, item->ptr);
    result = malloc(sizeof(struct keyvalue));
    result->key    = key;
    result->data   = item->data;
    result->length = item->length;

    DEBUG(printdata("lru found: ", item->data, item->length));
    DEBUG(printf("\n"));

    return result;
  }

  return NULL;
}

/**
   Search the table for the key/data pair and update the data.
   Copies data out of value: does not free it.
   Frees old data pointer.
   @return True iff the item was found and updated.
*/
bool
lru_table_update(struct lru_table* table,
                 char* key, char* value, int offset, int length)
{
  SHOW_FSII(key,offset,length);

  struct dpkm_list_item* item = NULL;
  int index = hash_string(key, table->capacity);

  item = dpkm_list_search(table->array[index], key);
  if (item)
    dpkm_list_pushback(table->lru, item->ptr);
  else
    return false;

  gossip_do(MASK_ADTS, lru_table_printf("%s", table));
  DEBUG(printdata("old data: ", item->data, item->length));

  int new_length = max(item->length, offset+length);

  char* update = realloc(item->data, new_length);
  assert(update);
  memcpy(update+offset, value, length);

  item->data = update;
  item->length = new_length;
  item = item->ptr;
  item->data = update;
  item->length = new_length;

  DEBUG(printdata("new data: ", item->data, item->length));

  DEBUG(lru_table_printdata(table));

  return true;
}

/**
   Remove the oldest key/value pair.

   @return NULL if the table is empty or a
           struct keyvalue* representing the key/data pair
           that was LRU.
 */
struct keyvalue*
lru_table_poll(struct lru_table* table)
{
  // NOTE_F;

  struct keyvalue* result = NULL;

  struct dpkm_list_item* oldest = dpkm_list_poll(table->lru);

  if (oldest == NULL)
  {
    return NULL;
  }
  else
  {
    int oldest_index = hash_string(oldest->key, table->capacity);
    dpkm_list_excise(table->array[oldest_index], oldest->ptr);
    result = (struct keyvalue*) malloc(sizeof(struct keyvalue));
    // SHOW_S(oldest->key);
    result->key    = oldest->key;
    result->data   = oldest->data;
    result->length = oldest->length;
    free(oldest->ptr);
    free(oldest);
    table->size--;
  }

  return result;
}

/**
   Internal method to remove key from table, not lru list.
*/
/*
bool
lru_table_remove(struct lru_table* target, struct dpkm_list_item* ptr)
{

list_excise()
}
*/

/**
   Write the table to stdout.
   @param format specifies the output format for the data items
 */
void
lru_table_dump(char* format, struct lru_table* target)
{
  int i;
  printf("LRU_TABLE(%i) \n{\n", target->size);
  printf("LRU: ");
  dpkm_list_printf(format, target->lru);
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size > 0)
    {
      printf(" %i: ", i);
      dpkm_list_printf(format, target->array[i]);
    }
  printf("}\n");
}

/**
   @param format specifies the output format for the data items
 */
void
lru_table_printf(char* format, struct lru_table* target)
{
  int i;
  printf("LRU_TABLE(%i) \n{\n", target->size);
  printf("LRU: ");
  dpkm_list_printf(format, target->lru);
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size > 0)
    {
      printf(" %i: ", i);
      dpkm_list_printf(format, target->array[i]);
    }
  printf("}\n");
}

/**
   @param format specifies the output format for the data items
 */
void
lru_table_fprintf(FILE* file, char* format, struct lru_table* target)
{
  int i;
  fprintf(file, "LRU_TABLE(%i) \n{\n", target->size);
  fprintf(file, "LRU: ");
  dpkm_list_fprintf(file, format, target->lru);
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size > 0)
    {
      fprintf(file, " %i: ", i);
      dpkm_list_fprintf(file, format, target->array[i]);
    }
  fprintf(file, "}\n");
}

/*
void
lru_table_dumpkeys(struct lru_table* target)
{
  int i;
  printf("{\n");
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size)
    {
      printf("%i:", i);
      dpkm_list_dumpkeys(target->array[i]);
    }
  printf("}\n");
}
*/

/**
 */
void
lru_table_fprintdata(FILE* file, struct lru_table* target)
{
  int i;
  fprintf(file, "LRU_TABLE(%i) \n{\n", target->size);
  fprintf(file, "LRU: ");
  dpkm_list_fprintdata(file, target->lru);
  for (i = 0; i < target->capacity; i++)
    if (target->array[i]->size > 0)
    {
      fprintf(file, " %i: ", i);
      dpkm_list_fprintdata(file, target->array[i]);
    }
  fprintf(file, "}\n");
}

void
lru_table_printdata(struct lru_table* target)
{
  lru_table_fprintdata(stdout, target);;
}


/** Dump dpkm_list to string a la snprintf()
    size must be greater than 2.
    format specifies the output format for the data items
    internally allocates O(size) memory
    returns int greater than size if size limits are exceeded
            indicating result is garbage
*/
int lru_table_tostring(char* str, size_t size,
                       char* format, struct lru_table* target)
{
  int   error = size+1;
  char* ptr   = str;
  int i;
  ptr += sprintf(str, "{\n");

  char* s = (char*) malloc(sizeof(char) * size);

  for (i = 0; i < target->capacity; i++)
  {
    int r = dpkm_list_tostring(s, size, format, target->array[i]);
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

#ifdef DEBUG_LRU_TABLE
int
main()
{
  struct lru_table* table = lru_table_create(11);

  lru_table_add(table, "key1", "val1");

  lru_table_dump("%s", table);

}
#endif
