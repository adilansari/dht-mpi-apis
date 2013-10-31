
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <accessor.h>

int
main(int argc, char* argv[])
{
  char msg[100];
  msg[0] = '\0';

  accessor_fifo_setup(NULL);
  bool success = accessor_fifo_init();
  assert(success);

  fprintf(to_cmpi,  "quit\n");
  fflush(to_cmpi);

  int count = fscanf(from_cmpi, "%s\n", msg);

  if (count != 1 ||
      strcmp(msg, "ok") != 0)
  {
    printf("error in cmpi-db-fifo-quit!\n");
    printf("received: %s\n", msg);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
