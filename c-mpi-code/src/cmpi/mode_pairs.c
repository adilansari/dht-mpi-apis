
#include <cmpi.h>
#include <cmpi_mode.h>

/**
   Pairs:
   Try to place one node and one client on each physical
   compute node
*/
int cmpi_mode_select(int rank, int size, int nodes)
{
  NOTE("using mode_rr");
  if (size%2 == 1)
  {
    printf("MPI size must be even for mode_rr!\n");
    exit(1);
  }

  if (rank >= size/2)
    return CMPI_MODE_CLIENT;
  return CMPI_MODE_NODE;
}

struct inlist* cmpi_mode_contacts(int rank, int size, int nodes)
{
  struct inlist* result = inlist_create();

  int i = rank % (size/2);

  NOTE_I("contact: ", i);
  inlist_add(result, i);

  return result;
}


int cmpi_mode_next(int rank, int size, int nodes)
{
  int result;
  int mode = cmpi_mode_select(rank, size, nodes);

  if (mode == CMPI_MODE_NODE)
  {
    int next = rank+1;
    if (next >= size/2)
      result = -1;
    else
      result = next;
  }
  else
  {
    int next = rank+1;
    if (next >= size)
      result = -1;
    else
      result = next;
  }

  return result;
}

int cmpi_mode_first_client(int rank, int size, int nodes)
{
  return size/2;
}

/**
   Obtain the list of nodes that this rank is
   responsible for instructing to quit.
*/
struct inlist* cmpi_shutdown_list(int rank, int size, int nodes)
{
  struct inlist* result = cmpi_mode_contacts(rank, size, nodes);

  return result;
}
