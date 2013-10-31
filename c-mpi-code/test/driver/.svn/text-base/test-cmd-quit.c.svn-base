
#include <stdio.h>
#include <stdlib.h>

#include <mpi_tools.h>
#include <accessor.h>

#include "test-cmd.h"

int
main(int argc, char* argv[])
{
  int error;
  int t;

  test_cmd_setup(argc, argv);

  error = accessor_fifo_init();
  assert(error);

  fprintf(to_cmpi,  "quit\n");
  fflush(to_cmpi);
  char output[32];
  t = fscanf(from_cmpi, "%s", output);

  assert(!strcmp(output, "ok"));

  return 0;
}
