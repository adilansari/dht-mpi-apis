
/**
 * Test dpkm_list functionality.
 * */

#include <dpkm_list.h>

#include "test_helpers.h"

void extra(struct dpkm_list_item* item)
{
  if (item)
  {
    printf("extra: %s\n", item->key);
    free(item);
  }
}

void extra_data(struct dpkm_list_item* item)
{
  if (item)
  {
    printf("extra: %s\n", item->key);
    printdata("data: ", item->data, strlen(item->data));
    printf("\n");
    free(item->data);
    free(item);
  }
}

void extra_key_data(struct dpkm_list_item* item)
{
  if (item)
  {
    printf("extra: %s\n", item->key);
    printdata("data: ", item->data, item->length);
    printf("\n");
    free(item->key);
    free(item->data);
    free(item);
  }
}

int
main(int argc, char* argv[])
{
  struct dpkm_list* L = dpkm_list_create(4);

  char s[1000];

  /// Begin tests with stack data...
  int zero  = 0;
  int two   = 2;
  int three = 3;
  int four  = 4;
  int five  = 5;
  int eight = 8;

  struct dpkm_list_item* item;

  item = dpkm_list_add(L, "two", &two, sizeof(int));
  extra(item);
  item = dpkm_list_add(L, "four", &four, sizeof(int));
  extra(item);
  item = dpkm_list_add(L, "three", &three, sizeof(int));
  extra(item);
  item = dpkm_list_add(L, "three", &three, sizeof(int));
  extra(item);
  item = dpkm_list_add(L, "four", &four, sizeof(int));
  extra(item);
  item = dpkm_list_add(L, "zero", &zero, sizeof(int));
  extra(item);

  dpkm_list_printf("%i", L);
  printf("size: %i \n", L->size); // 4

  item = dpkm_list_add(L, "five", &five, sizeof(int));
  extra(item);
  item = L->head;
  dpkm_list_excise(L, L->head);
  extra(item);
  dpkm_list_printf("%i", L);
  printf("size: %i \n", L->size); // 3

  item = dpkm_list_add(L, "eight", &eight, sizeof(int));
  extra(item);
  dpkm_list_printf("%i", L);
  printf("size: %i \n", L->size); // 4

  dpkm_list_tostring(s, 1000, "%i", L);
  printf("From string: %s\n", s);

  item = dpkm_list_poll(L);
  extra(item);
  dpkm_list_printf("%i", L);
  printf("size: %i \n", L->size); // 3

  item = dpkm_list_search(L, "zero");
  dpkm_list_excise(L, item);
  extra(item);
  dpkm_list_printf("%i", L);

  puts("polling...");

  while ((item = dpkm_list_poll(L)))
  {
    printf("got: %s \n", item->key);
    extra(item);
    dpkm_list_printf("%i", L);
  }

  dpkm_list_tostring(s, 1000, "%i", L);
  printf("From string: %s\n", s);

  assert(L->size == 0);
  dpkm_list_free(L);

  struct dpkm_list* L2 = dpkm_list_create(4);
  char* binary = heap("data");
  dpkm_list_add(L2, "binary", binary, strlen(binary)+1);
  dpkm_list_printdata(L2);

  item = dpkm_list_poll(L2);
  extra_data(item);

  for (int i = 0; i < 30; i++)
  {
    int j = i % 10;
    if (rand() % 2)
    {
      dpkm_list_printdata(L2);
      char* key = makekey(j);
      void* data = makedata(j);
      printf("adding: %s\n", key);
      item = dpkm_list_add(L2, key, data, j);
      extra_key_data(item);
    }
  }
  dpkm_list_printdata(L2);
  printf("size: %i\n", L2->size);

  while ((item = dpkm_list_poll(L2)))
    extra_key_data(item);

  printf("size: %i\n", L2->size);
  assert(L2->size == 0);
  dpkm_list_free(L2);
}
