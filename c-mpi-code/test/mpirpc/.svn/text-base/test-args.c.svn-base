
/**
   Simple two-processor argument test
   for NULL, 0-length, and non-0-length args.
   NULL return values.
*/

#include <mpi_tools.h>
#include <gossip.h>
#include <mpirpc.h>

void
handle_test(MPIRPC_Node caller, int unique, char* args,
            void* blob, int blob_length)
{
  show_fs(args);
  MPIRPC_Null(caller, unique);
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
    MPIRPC_Node_make(MPI_COMM_WORLD, 1, &neighbor);
    MPIRPC_Block(neighbor, "test", NULL);
    MPIRPC_Block(neighbor, "test", "");
    MPIRPC_Block(neighbor, "test", "alpha");
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

  DMALLOC_COMPLETE;

  return 0;
}
