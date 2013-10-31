
/**
   Test math functions and simple output.
*/

#include <mpi_tools.h>

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);

  whoami();
  setup_debug_file();

  int i;
  for (i = 0; i < 20; i++)
  {
    SHOW_II(i, ilog2(i));
    SHOW_II(i, isqrt(i));
  }

  MPI_Finalize();

  return 0;
}
