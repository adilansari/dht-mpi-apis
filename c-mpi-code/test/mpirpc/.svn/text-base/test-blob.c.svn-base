
/**
   Simple two-processor argument test
   for non-0-length args and
   for non-0-length blobs.
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
  printf("handle_test: %i\n", a);

  assert(blob_length == 4);
  char* data = (char*) blob;

  assert(data[0] == '\n');
  assert(data[1] == '\0');
  assert(data[2] == '\t');
  assert(data[3] == '\n');

  printf("asserts ok\n");

  MPIRPC_Null(caller, unique);
  free(blob);
}

int
main(int argc, char* argv[])
{
  gossip_set_debug_mask(1, MASK_MPIRPC);

  MPI_Init(&argc, &argv);
  MPIRPC_Init();

  MPIRPC_Comm_add(MPI_COMM_WORLD);

  MPIRPC_Node neighbor;
  if (mpi_rank == 0)
  {
    char blob[4];
    blob[0] = '\n';
    blob[1] = '\0';
    blob[2] = '\t';
    blob[3] = '\n';
    char* result;
    MPIRPC_Node_make(MPI_COMM_WORLD, 1, &neighbor);
    result = MPIRPC_Block_blob(neighbor, "test", "0", blob, 4);
    printf("result: %s\n", result);
  }
  else
  {
    MPIRPC_Node_make(MPI_COMM_WORLD, 0, &neighbor);
    MPIRPC_Register("test", handle_test);
    while (! MPIRPC_Check());
  }

  printf("Normal exit.\n");
  MPIRPC_Finalize();
  MPI_Finalize();
}
