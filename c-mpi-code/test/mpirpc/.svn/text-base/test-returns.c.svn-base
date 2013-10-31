
/**
   Simple two-processor argument test
   for non-0-length args and
   for NULL, 0-length, and non-0-length return values.
*/

#include <mpi_tools.h>
#include <gossip.h>

#include <mpirpc.h>

#include "test_helpers.h"

void
handle_test(MPIRPC_Node caller, int unique, char* args,
            void* blob, int blob_length)
{
  int a;
  sscanf(args, "%i", &a);
  show_fsi(args, a);
  char* result = malloc(4);
  SHOW_P(result);

  if (a == 0)
  {
    MPIRPC_Null(caller, unique);
  }
  else if (a == 1)
  {
    MPIRPC_Return(caller, unique, result, 0);
  }
  else if (a == 2)
  {
    strcpy(result, "ok\n");
    MPIRPC_Return(caller, unique, result, 4);
  }
}

int
main(int argc, char* argv[])
{
  gossip_set_debug_mask(1, MASK_MPIRPC);

  MPI_Init(&argc, &argv);
  MPIRPC_Init();
  whoami();
  setup_debug_file();

  MPIRPC_Comm_add(MPI_COMM_WORLD);

  MPIRPC_Node neighbor;
  if (mpi_rank == 0)
  {
    char* result;
    MPIRPC_Node_make(MPI_COMM_WORLD, 1, &neighbor);
    result = MPIRPC_Block(neighbor, "test", "0");
    show_s(result);
    if (result)
      free(result);
    result = MPIRPC_Block(neighbor, "test", "1");
    show_s(result);
    if (result)
      free(result);
    result = MPIRPC_Block(neighbor, "test", "2");
    show_s(result);
    if (result)
      free(result);
  }
  else
  {
    MPIRPC_Node_make(MPI_COMM_WORLD, 0, &neighbor);
    MPIRPC_Register("test", handle_test);
    while (! MPIRPC_Check());
    while (! MPIRPC_Check());
    while (! MPIRPC_Check());
  }

  printf("Normal exit.\n");
  MPIRPC_Finalize();
  MPI_Finalize();

  return 0;
}
