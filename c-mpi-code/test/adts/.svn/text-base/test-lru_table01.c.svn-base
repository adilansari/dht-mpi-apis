
/**
 * Test lru_table functionality.
 * */

#include <lru_table.h>

#include "test_helpers.h"

void extra_kv(struct keyvalue* kv)
{
  if (kv)
  {
    printf("extra: %s \n", kv->key);
    free(kv);
  }
}

/**
   Clean up extra keyvalue and its key and data.
*/
void extra_kv_kd(struct keyvalue* kv)
{
  if (kv)
  {
    printf("extra: %s \n", kv->key);
    if (kv->data)
      free(kv->data);
    if (kv->key)
      free(kv->key);
    free(kv);
  }
}

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);
  whoami();

  struct lru_table* table = lru_table_create(5, 4);

  int* one   = malloc(sizeof(int));
  int* two   = malloc(sizeof(int));

  *one   = 1;
  *two   = 2;

  struct keyvalue* kv;

  kv = lru_table_add(table, "one", one, sizeof(int));
  lru_table_printf("%i", table);
  extra_kv(kv);

  printf("\n");

  kv = lru_table_add(table, "two", two, sizeof(int));
  lru_table_printf("%i", table);
  extra_kv(kv);

  printf("\n");

  kv = lru_table_search(table, "one");
  assert(kv);
  printf("data: %i\n", *(int*) kv->data);
  free(kv);

  kv = lru_table_search(table, "two");
  assert(kv);
  printf("data: %i\n", *(int*) kv->data);
  free(kv);

  kv = lru_table_search(table, "one");
  assert(kv);
  printf("data: %i\n", *(int*) kv->data);
  free(kv);

  while ((kv = lru_table_poll(table)))
  {
    printf("polled: %s %i\n", kv->key, *kv->data);
    free(kv);
  }

  assert(table->size == 0);
  lru_table_free(table);

  free(one);
  free(two);

  printf("loop...");
  table = lru_table_create(6, 4);
  for (int i = 0; i < 30; i++)
  {
    lru_table_printdata(table);
    char* key  = makekey(i);
    if (rand() % 2)
    {
      void* data = makedata(i);
      kv = lru_table_add(table, key, data, i);
      extra_kv_kd(kv);
    }
    else
    {
      printf("search: %s\n", key);
      kv = lru_table_search(table, key);
      free(key);
      extra_kv(kv);
    }
  }

  puts("draining...");
  while ((kv = lru_table_poll(table)))
  {
    printf("polled: %s\n", kv->key);
    printdata("", kv->data, kv->length);
    keyvalue_destroy(kv);
  }

  assert(table->size == 0);
  lru_table_free(table);

  /*
  int* result = (int*) lru_table_search(table, "five");
  printf("result: %i \n", *result);
  lru_table_dump("%i", table);

  result = (int*) lru_table_search(table, "twenty");
  if (result == NULL)
    printf("Got NULL \n");

  result = (int*) lru_table_search(table, "five");
  printf("result: %i \n", *result);
  lru_table_dump("%i", table);

  while ((kv = lru_table_poll(table)))
    printf("polled: %s \n", kv->key);
  */


  /*

  lru_table_add(table, "five", &five);
  lru_table_excise(table, table->head);
  lru_table_dump("%i", table);
  printf("size: %i \n", table->size);

  kv = lru_table_add(table, "eight", &eight);
  if (kv)
    printf("%s \n", kv->key);
  lru_table_dump("%i", table);
  printf("size: %i \n", table->size);

  lru_table_touch(table, table->head->next);
  lru_table_dump("%i", table);
  printf("size: %i \n", table->size);

  struct lru_table_item* item;
  item = lru_table_poll(table);
  lru_table_dump("%i", table);
  printf("size: %i \n", table->size);

  while ((item = lru_table_poll(table)))
    lru_table_dump("%i", table);

  */
  /*
  lru_table_dump("%i", table);
  lru_table_poll(table);
  lru_table_dump("%i", table);
  lru_table_pop(table);
  lru_table_dump("%i", table);
  lru_table_add(table, &seven);
  lru_table_add(table, &six);
  lru_table_dump("%i", table);
  */

  // lru_table_clobber(table);
  // lru_table_clear(table);
  // printf("size(table): %i \n", table->size);

  // lru_table_clear(K);
  // printf("size(K): %i \n", K->size);

  // lru_table_dump("%i", table);

  // dmalloc_shutdown();

  printf("\n");
  printf("Normal exit.\n");

  MPI_Finalize();

  return 0;
}
