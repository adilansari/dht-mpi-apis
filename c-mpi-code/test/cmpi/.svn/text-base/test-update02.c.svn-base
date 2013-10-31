
/**
   Update two new keys, update those, and retrieve one.
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

  char* value = malloc(64*sizeof(char));

  sprintf(value, "value1");
  cmpi_update(key1, value, strlen(value)+1, 0);
  sleep(1);
  sprintf(value, "value2");
  cmpi_update(key2, value, strlen(value)+1, 0);
  sleep(1);
  sprintf(value, "X");
  cmpi_update(key2, value, strlen(value)+1, 3);
  sleep(1);
  sprintf(value, "value3");
  cmpi_update(key3, value, strlen(value)+1, 0);
  sleep(1);
  sprintf(value, "X");
  cmpi_update(key3, value, strlen(value)+1, 1);
  sleep(1);
  sprintf(value, "value1");
  cmpi_update(key3, value, strlen(value)+1, 3);

  free(value);

  sleep(2);

  void* result;
  int length;
  cmpi_get(key1, &result, &length);
  printf("RESULT_1: %s\n", (char*) result);
  sleep(1);
  cmpi_get(key2, &result, &length);
  printf("RESULT_2: %s\n", (char*) result);
  sleep(1);
  cmpi_get(key3, &result, &length);
  printf("RESULT_3: %s\n", (char*) result);
  NOTE("GET SUCCEEDED");

  sleep(2);

  cmpi_shutdown();
}
