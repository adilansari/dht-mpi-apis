
/**
   Put three keys, update one, and get the updated value.
*/

#include <unistd.h>

#include "test_helpers.h"

void
cmpi_client_code()
{
  char key1[10];
  char key2[10];
  char key3[10];
  sprintf(key1, "key1_%i", mpi_rank);
  sprintf(key2, "key2_%i", mpi_rank);
  sprintf(key3, "key3_%i", mpi_rank);

  wait_for_notification();
  notify_next();

  char value[30];
  sprintf(value, "value1");
  cmpi_put(key1, value, strlen(value)+1);
  printf("FIRST PUT SUCCEEDED\n");
  sprintf(value, "value2");
  cmpi_put(key2, value, strlen(value)+1);
  printf("SECOND PUT SUCCEEDED\n");
  sprintf(value, "value3");
  cmpi_put(key3, value, strlen(value)+1);
  printf("THIRD PUT SUCCEEDED\n");

  sleep(2);

  cmpi_update(key2, "X", 1, 3);

  void* result;
  int   length;
  cmpi_get(key2, &result, &length);
  printf("result: %s\n", (char*) result);
  printf("GET SUCCEEDED\n");

  sleep(2);

  cmpi_shutdown();
}
