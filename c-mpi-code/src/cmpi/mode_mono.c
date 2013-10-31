
#include <cmpi.h>
#include <cmpi_mode.h>

/**
   Original idea: if rank > nodes, I am client.
*/
int cmpi_mode_select(int rank, int size, int nodes)
{
  if (rank >= nodes)
    return CMPI_MODE_CLIENT;
  return CMPI_MODE_NODE;
}

struct inlist* cmpi_mode_contacts(int rank, int size, int nodes)
{
  struct inlist* result = inlist_create();

  for (int i = 0; i < nodes; i++)
  {
    NOTE_I("contact: ", i);
    inlist_add(result, i);
  }

  return result;
}

int cmpi_mode_next(int rank, int size, int nodes)
{
  int result;
  int mode = cmpi_mode_select(rank, size, nodes);

  if (mode == CMPI_MODE_NODE)
  {
    int next = rank+1;
    if (next > nodes-1)
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
  SHOW_FI(nodes);
  return nodes;
}

/**
   Obtain the list of nodes that this rank is
   responsible for instructing to quit.
   Should only be called by clients.
*/
struct inlist* cmpi_shutdown_list(int rank, int size, int nodes)
{
  NOTE_F;

  struct inlist* result = inlist_create();

  int mode = cmpi_mode_select(rank, size, nodes);
  assert(mode != CMPI_MODE_NODE);

  int first = cmpi_mode_first_client(rank, size, nodes);
  if (rank == first)
    for (int i = 0; i < nodes; i++)
      inlist_add(result, i);

  return result;
}
