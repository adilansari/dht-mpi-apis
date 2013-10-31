
#include "test_helpers.h"
#include <cmpi_mode.h>

/**
   Print debugging header using machine-specific information.
*/
void
print_header(void)
{
  // Are we a SLURM job?
  char* slurm_id = getenv("SLURM_JOBID");
  if (slurm_id)
    printf("SLURM_JOBID: %s\n", slurm_id);
}

char*
useless_data(int size)
{
  int i;
  char* s = malloc(size*sizeof(char));
  for (i = 0; i < size-1; i++)
    s[i] = 'a' + rand_lt(26);
  s[size-1] = '\0';
  return s;
}

void
wait_for_notification(void)
{
  int tmp;
  MPI_Status status;
  NOTE_F;
  MPI_Recv(&tmp, 1, MPI_INT,
           MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  assert(tmp == -2);
}

void
notify_next(void)
{
  NOTE_F;
  int next = cmpi_mode_next(mpi_rank, mpi_size, cmpi_nodes);
  if (next >= 0)
  {
    int msg = -2;
    MPI_Send(&msg, 1, MPI_INT,
             mpi_rank+1, 0, MPI_COMM_WORLD);
  }
  else
  {
    NOTE("NOTIFICATION_FINALIZED");
  }
}
