
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <accessor.h>

#include "test-cmd.h"

int
main(int argc, char* argv[])
{
  int code;
  int t;

  test_cmd_setup(argc, argv);

  code = accessor_fifo_init();
  assert(code);

  fprintf(to_cmpi,  "sleep %i\n", sleep_time);
  fflush(to_cmpi);
  char output[32];
  t = fscanf(from_cmpi, "%s", output);
  assert(t == 1);

  if (!(output[0] == 'o' &&
        output[1] == 'k'))
  {
    printf("bad output: %s\n", output);
    exit(1);
  }

  return 0;
}
