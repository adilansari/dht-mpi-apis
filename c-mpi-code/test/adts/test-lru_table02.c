
/**
 * Test lru_table update functionality.
 * */

#include <lru_table.h>

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);
  whoami();

#ifdef DMALLOC
  printf("DMALLOC\n");
  // sprintf(dmalloc_logpath, "%s", "./dmalloc.out");
#endif

  struct lru_table* table = lru_table_create(5, 4);
  struct keyvalue* kv;

  lru_table_printf("%s", table);
  char* data1 = "data1";
  char* data2 = "data2";
  lru_table_add(table, "key1", heap(data1), strlen(data1)+1);
  lru_table_printf("%s", table);
  lru_table_add(table, "key2", heap(data2), strlen(data2)+1);
  lru_table_printf("%s", table);

  char* overwrite = "_-_-_";
  lru_table_update(table, "key2", overwrite, 3, strlen(overwrite)+1);
  lru_table_printf("%s", table);

  overwrite = "X";
  lru_table_update(table, "key2", overwrite, 1, strlen(overwrite)+1);
  lru_table_printf("%s", table);

  while ((kv = lru_table_poll(table)))
    printf("polled: %s %s\n", kv->key, (char*) kv->data);

  lru_table_printf("%s", table);

  MPI_Finalize();
  return 0;
}
