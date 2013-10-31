
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <accessor.h>
#include <mpi_tools.h>
#include <io_tools.h>

#include "test-cmd.h"

void
crash(char* message)
{
  printf("%s\n", message);
  accessor_fifo_close();
  exit(1);
}

int
main(int argc, char* argv[])
{
  int code;
  char* t;

  test_cmd_setup(argc, argv);

  char* key = argv[optind];

  code = accessor_fifo_init();
  assert(code);

  fprintf(to_cmpi, "get %s\n", key);
  fflush(to_cmpi);

  int length;
  char message[32];
  t = fgets(message, 32, from_cmpi); // &length);
  // printf("length: %s\n", message);
  // if (code != 1)
  //  crash("No response from driver!");
  // printf("length: %i code: %i \n", length, code);

  sscanf(message, "%i", &length);

  if (length == -1)
    crash("Key not found!");

  // printf("read stream\n");
  char* result = malloc(length*sizeof(char));
  memset(result, 0, length);
  stream_to_buffer(result, length, from_cmpi);

  if (strcmp(result, "hello"))
    crash("wrong result!");

  return 0;
}
