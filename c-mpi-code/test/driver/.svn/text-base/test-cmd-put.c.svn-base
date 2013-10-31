
#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <accessor.h>

#include "test-cmd.h"

int
main(int argc, char* argv[])
{
  test_cmd_setup(argc, argv);

  accessor_fifo_init();

  char* key = argv[optind];

  char* data = "hello";

  accessor_put(key, data, strlen(data)+1);

  return 0;
}
