
/**
   Store many small records.
   The number of records is controlled by the
   "reps" key in the CMPI cache.
*/

#include <unistd.h>

#include "test_helpers.h"

/**
   Requires that "reps" be in the CMPI params.
   Generates keys for multiple clients.
*/
void
cmpi_client_code()
{
  int i;

  // Obtain the number of reps to perform:
  char* s = cmpi_params_get("reps");
  if (s == NULL || strlen(s) == 0)
  {
    puts("No reps given!");
    exit(1);
  }
  int reps;
  int n = sscanf(s, "%i", &reps);
  assert(n > 0);

  wait_for_notification();
  notify_next();

  MPI_Barrier(cmpi_comm_clients);

  double start = MPI_Wtime();

  for (i = 0; i < reps; i++)
  {
    char key[20];
    sprintf(key, "key_%i_%i", mpi_rank, i);
    char* value = useless_data(10);

    cmpi_put(key, value, strlen(value)+1);
    free(value);
  }
  double stop = MPI_Wtime();

  // sleep((mpi_size-mpi_rank)*3+10);
  MPI_Barrier(cmpi_comm_clients);

  sleep(1);
  if (mpi_rank + 1 == mpi_size)
    printf("TOOK: %0.3f\n", stop-start);

  cmpi_shutdown();
}
