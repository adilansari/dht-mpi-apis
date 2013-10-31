
/**
 * Test inlist functionality.
 * */

#include <mpi.h>

#include <inlist.h>

#include "test_helpers.h"

int
main(int argc, char* argv[])
{
  char* s = " 1  4 2  ";

  struct inlist* L = inlist_parse(s);

  inlist_printf(L);

  inlist_free(L);

  return 0;
}
