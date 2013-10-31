
/**
 * Participant client for CMPI.
 * Only used in multiple communicator, MPI-2 case.
 *
 * Command-line arguments:
 *
 * -s <ms>  snooze_max in milliseconds.
 * -t <tag> symbolic tag number
 */

#include "node.h"

int nodes;

void
options(int argc, char* argv[])
{
  struct option options[] =
    {
      {"snooze",  required_argument, NULL, 's'},
      {"tag",     required_argument, NULL, 't'},
      {"nodes",   required_argument, NULL, 'n'},
      {0, 0, 0, 0}
    };

  int c = 0;
  int t = 0;
  while ((c = getopt_long(argc, argv, "f:s:t:n:c:", options, &c)) != -1)
  {
    switch (c)
    {
      case 's':
        sscanf(optarg, "%i", &t);
        snooze_max = t*1000;
        // printf("Snooze max: %i \n", snooze_max);
      case 't':
        sscanf(optarg, "%i", &t);
        debug_rank = t;
      case 'n':
        sscanf(optarg, "%i", &t);
        nodes = t;
    }
    c++;
  }
}

int
main(int argc, char* argv[])
{
  options(argc, argv);

  MPI_Init(&argc, &argv);

  whoami();
  if (mpi_rank == 0)
    timestamp("START", NULL);

  cmpi_init_client();
  cmpi_client_code();

  MPI_Finalize();

  return 0;
}
