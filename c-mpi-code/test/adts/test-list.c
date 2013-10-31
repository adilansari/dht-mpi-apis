
/**
 * Test list functionality.
 * */

#include <list.h>

int
main(int argc, char* argv[])
{
  struct list* L;

  MPI_Init(&argc, &argv);
  whoami();

  L = list_parse("jkl iop l");

  list_printf("%s", L);
  fflush(stdout);
  list_destroy(L);

  L = list_parse(" jkl iop  l  ");
  list_printf("%s", L);
  list_destroy(L);

  MPI_Finalize();
  return 0;
}
