
/**
   Put two keys and get one of them.
*/

#include <unistd.h>

#include "test_helpers.h"

void
cmpi_client_code()
{
  NOTE_F;

  char key1[10];
  char key2[10];

  sprintf(key1, "key1_%i", mpi_rank);
  sprintf(key2, "key2_%i", mpi_rank);

  sleep(mpi_rank);

  char value[30];
  strcpy(value, "value1");
  cmpi_put(key1, value, strlen(value)+1);
  strcpy(value, "value2");
  cmpi_put(key2, value, strlen(value)+1);

  sleep(2);

  cmpi_debug_caches();

  sleep(2);

  int   length;
  void* result;
  CMPI_RETURN code = cmpi_get(key1, &result, &length);
  cmpi_assert(code, "get() failed!");
  printf("result(%i): %s\n", length, (char*) result);
  free(result);

  sleep(3);

  cmpi_shutdown();
}
