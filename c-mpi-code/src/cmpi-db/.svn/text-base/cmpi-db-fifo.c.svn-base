
#include <unistd.h>

#include <node.h>
#include <driver.h>

void
wait_for_notification(void)
{
  int tmp;
  MPI_Status status;
  MPI_Recv(&tmp, 1, MPI_INT,
           MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
  if (tmp != -2)
  {
    NOTE("notification error!\n");
    exit(1);
  }
}

void
notify_next(void)
{
  if (mpi_rank < mpi_size-1)
  {
    int msg = -2;
    NOTE_I("notifying: ", mpi_rank+1);
    MPI_Send(&msg, 1, MPI_INT, mpi_rank+1, 0, MPI_COMM_WORLD);
  }
  else
  {
    printf("cmpi-db ready\n");
  }
}

void
cmpi_client_code()
{
  char hostname[128];

  char* s = getenv("DMALLOC_OPTIONS");

  // printf("cmpi_client_code()\n");
  NOTE("CMPI-DB-FIFO...");

  printf("DMALLOC_OPTIONS: %s\n", s);

  wait_for_notification();
  notify_next();

  gethostname(hostname, 100);
  SHOW_S(hostname);

  cmpi_driver* driver = driver_create();

  while (! driver->quitting)
  {
    driver_setup_fifo(driver);
    driver_process(driver);
  }
}
