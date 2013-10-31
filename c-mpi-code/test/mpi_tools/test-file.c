
#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>

#include <io_tools.h>
#include <mpi_tools.h>

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);

  char* buffer;

  int total = file_to_buffer(&buffer, "setup.sh");

  //printf("got buffer: %i\n", total);

  if (total < 0)
    exit(1);

  fwrite(buffer, sizeof(char), total, stdout);
  //printdata("data: ", buffer, 4);
  //printf("\n");

  MPI_Finalize();

  return 0;
}
