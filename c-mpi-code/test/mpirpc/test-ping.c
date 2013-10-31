/**
 Simple two-processor ping pong test.
 No arguments or return values.
 */

#include <unistd.h>

#include <mpi_tools.h>
#include <gossip.h>
#include <mpirpc.h>

#include "test_helpers.h"

int count = 0;
bool running = true;

void
handle_ping(MPIRPC_Node caller, int unique, char* args, void* blob,
            int blob_length)
{
  puts("handle_ping");
  MPIRPC_Null(caller, unique);
  if (++count < 3)
  {
    MPIRPC_Call(caller, "ping", NULL, NULL, MPIRPC_Free);
  }
  else
  {
    MPIRPC_Flush_returns();
    MPIRPC_Block(caller, "quit", NULL);
    running = false;
  }
}

void
handle_quit(MPIRPC_Node caller, int unique, char* args, void* blob,
            int blob_length)
{
  puts("handle_quit");
  running = false;
  MPIRPC_Null(caller, unique);
}

int
main(int argc, char* argv[])
{
  printf("hello\n");
  // printf("host: %s\n", file_string("/etc/hostname"));

  MPI_Init(&argc, &argv);

  whoami();
  setup_debug_file();

  MPIRPC_Init();

  SHOW_I(debug_rank);

  MPIRPC_Comm_add(MPI_COMM_WORLD);
  MPIRPC_Register("ping", handle_ping);
  MPIRPC_Register("quit", handle_quit);

  MPIRPC_Node neighbor;
  if (mpi_rank == 0)
  {
    MPIRPC_Node_make(MPI_COMM_WORLD, 1, &neighbor);
    MPIRPC_Block(neighbor, "ping", NULL);
  }
  else
  {
    MPIRPC_Node_make(MPI_COMM_WORLD, 0, &neighbor);
  }

  while (running)
  {
    MPIRPC_Check();
    sleep(1);
  }

  note("Normal exit.\n");

  MPIRPC_Finalize();
  MPI_Finalize();

  DMALLOC_COMPLETE;

  return 0;
}
