
/**
 * Participant node/client for CMPI.
 *
 * Command-line arguments:
 * -c <cap>   the capacity of the CMPI cache.
 * -n <nodes> the number of nodes to start as DHT nodes.
 * -p <k>=<v> store k,v in the CMPI parameter table.
 * -s <ms>    snooze_max in milliseconds.
 * -t <tag>   symbolic tag number.
 */

#include <getopt.h>
#include <unistd.h>

#include <node.h>
#include <cmpi_mode.h>
#include <io_tools.h>

int      cmpi_nodes;
MPI_Comm cmpi_comm_clients;

void
options(int argc, char* argv[])
{
  struct option options[] =
    {
      {"cache",   required_argument, NULL, 'c'},
      {"nodes",   required_argument, NULL, 'n'},
      {"param",   required_argument, NULL, 'p'},
      {"snooze",  required_argument, NULL, 's'},
      {"tag",     required_argument, NULL, 't'},
      {0, 0, 0, 0}
    };

  int   c = 0;
  int   t = 0;
  char* p;
  char key[40];
  char value[40];

  // Defaults...
  cmpi_cache_limit = 10000;
  int snooze_max       = 500000;
  debug_rank       = mpi_rank;
  cmpi_nodes       = mpi_size;

  cmpi_params_init();
  while ((c =
          getopt_long(argc, argv, "c:s:t:n:p:", options, &c)) != -1)
  {
    int n;
    switch (c)
    {
      case 'c':
        n = sscanf(optarg, "%i", &cmpi_cache_limit);
        if (n < 1)
        {
          printf("Got bad CMPI cache limit!\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'n':
        n = sscanf(optarg, "%i", &t);
        if (n < 1)
        {
          printf("Got bad node count!\n");
          exit(EXIT_FAILURE);
        }
        cmpi_params_add("nodes", optarg);
        cmpi_nodes = t;
        break;
      case 'p':
        p = strchr(optarg, '=');
        if (p == NULL)
        {
          printf("Got bad -p argument! \n\n");
          exit(EXIT_FAILURE);
        }
        *p = ' ';
        n = sscanf(optarg, "%s %s", key, value);
        if (n == 2)
          cmpi_params_add(key, value);
        else if (n == 1)
          cmpi_params_add(key, "");
        else
        {
          printf("Got bad -p argument! \n");
          exit(EXIT_FAILURE);
        }
        break;
      case 's':
        n = sscanf(optarg, "%i", &t);
        if (n < 1)
        {
          printf("Got bad snooze time!\n");
          exit(EXIT_FAILURE);
        }
        snooze_max = t*1000;
        // printf("Snooze max: %i \n", snooze_max);
        break;
      case 't':
        n = sscanf(optarg, "%i", &t);
        if (n < 1)
        {
          printf("Got bad debug rank!\n");
          exit(EXIT_FAILURE);
        }

        debug_rank = t;
        break;

      default:
      {
        printf("Got bad argument! \n");
        exit(EXIT_FAILURE);
      }
    }
    c++;
  }

  MPIRPC_Snooze_set(snooze_max);
}

/**
   Sets up cmpi_comm_clients.
*/
void
build_client_communicator(void)
{
  MPI_Group group_world;
  MPI_Group group_clients;
  int ranges[1][3];

  // gossip_debug(MASK_CMPI, ":\n");

  if (mpi_size == cmpi_nodes)
  {
    cmpi_comm_clients = MPI_COMM_NULL;
    return;
  }

  ranges[0][0] = cmpi_nodes;
  ranges[0][1] = mpi_size-1;
  ranges[0][2] = 1;

  MPI_Comm_group(MPI_COMM_WORLD, &group_world);
  MPI_Group_range_incl(group_world, 1, ranges, &group_clients);
  MPI_Comm_create(MPI_COMM_WORLD, group_clients, &cmpi_comm_clients);
}

int
main(int argc, char* argv[])
{
  gossip_debug_init();

  gossip_set_debug_mask(true,MASK_DEBUG|MASK_MPIRPC|// MASK_ADTS|
                              MASK_CMPI|MASK_DRIVER);

  gossip_enable_stdout();

  // Comment out to achieve deterministic results:
  // srand((unsigned int) time(NULL));

  // Basic MPI startup...
  MPI_Init(&argc, &argv);

  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  whoami();

  if (mpi_rank == 0)
    DEBUG(timestamp("START", NULL));

  // Handle and store options...
  options(argc, argv);

#if USE_COMM_WORLD == 1
// Use MPI_COMM_WORLD to bootstrap.

  build_client_communicator();

  sleep(1);

  char hostname[128];
  /*
  char* buffer;
  int length = file_to_buffer(&buffer, "/etc/hostname");
  memset(hostname, 0, 128);
  memcpy(hostname, buffer, length);
  NOTE_S("hostname: ", hostname);
  */
  int error = gethostname(hostname, 128);
  if (error)
  {
    printf("could not gethostname()!\n");
    exit(1);
  }

  char* tag = cmpi_params_get("tag");
  if (tag)
    note_s("tag: ", tag);

  int mode = cmpi_mode_select(mpi_rank, mpi_size, cmpi_nodes);
  if (mode == CMPI_MODE_NODE)
  {
    NOTE("I am node");
    printf("node: %i @ %s\n", mpi_rank, hostname);
    cmpi_init();
  }
  else
  {
    NOTE("I am client");
    printf("client: %i @ %s\n", mpi_rank, hostname);
    cmpi_init_client();
    cmpi_client_code();
  }

#else
  // Startup as a stand-alone node, will use MPI-2 to bootstrap.

  NOTE("NODE-2");
  cmpi_init();

#endif

  // NOTE("MPI_Finalize()...");
  MPI_Finalize();

#if USE_COMM_WORLD == 1

  if (mode == CMPI_MODE_NODE)
    cmpi_cleanup();
  else
    cmpi_client_cleanup();

#else

  cmpi_cleanup();

#endif


  DMALLOC_SETUP(// if (mpi_rank == 0)
                { NOTE("DMALLOC_SHUTDOWN");
                  SHOW_S(dmalloc_logpath);
                  dmalloc_shutdown();
                  dmalloc_log_unfreed();
                });

  NOTE("Normal exit.");
  return 0;
}
