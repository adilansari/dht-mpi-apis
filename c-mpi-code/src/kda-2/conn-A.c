
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#include <time.h>

#include "kda_conn-A.h"
#include "kda_neighbor-2.h"
#include "cmpi_mode.h"

/**
   Maps KDA_IDs to ints.
   Stores MPI ranks in MPI_COMM_WORLD for all known KDA_IDs.
*/
struct hashtable* world_ranks;

void
KDA_Init_conn()
{
  NOTE_F;
  // Add self to world_ranks...
  world_ranks = hashtable_create(mpi_size);
  int* heap_mpi_rank = malloc(sizeof(int));
  *heap_mpi_rank = mpi_size;
  hashtable_add(world_ranks, xheap(id), heap_mpi_rank);

  MPIRPC_Comm_add(MPI_COMM_WORLD);

  MPIRPC_Register("get_id", handle_get_id);
  MPIRPC_Register("get_rank", handle_get_rank);

  sleep(mpi_rank * 3);

  if (mpi_rank > 0)
  {
    int other_rank = rand_lt(mpi_rank);
    MPIRPC_Node node;
    MPIRPC_Node_make(MPI_COMM_WORLD, other_rank, &node);
    SHOW_I(other_rank);
    char* result = MPIRPC_Block(node, "get_id", NULL);
    KDA_ID other_id;
    sscanf(result, "%X", &other_id);
    free(result);
    KDA_Neighbor neighbor;
    KDA_Neighbor_make_id(other_id, node, &neighbor);
    neighbor_add(&neighbor);
    MPIRPC_Node dummy = {0};
    KDA_Join(dummy, &neighbor);
  }

  /*
  int client =
      cmpi_mode_first_client(mpi_rank, mpi_size, kda_nodes);
  SHOW_I(client);
  int msg = -2;
  // MPI_Send(&msg, 1, MPI_INT, client, 0, MPI_COMM_WORLD);
*/
  listen_loop();
}

/**
   Form client->node connections
*/
void
KDA_Attach(struct list* contacts)
{
  NOTE_F;
  MPIRPC_Comm_add(MPI_COMM_WORLD);
  for (int i = 0; i < 3; i++)
  {
    int other_rank = rand_lt(mpi_size/2);
    MPIRPC_Node node;
    MPIRPC_Node_make(MPI_COMM_WORLD, other_rank, &node);
    char* result = MPIRPC_Block(node, "get_id", NULL);
    NOTE("got id");
    KDA_ID other_id;
    sscanf(result, "%X", &other_id);
    free(result);
    KDA_Neighbor* neighbor = KDA_Neighbor_create_id(other_id, node);
    if (KDA_Attach_to(neighbor, contacts))
    {
      NOTE_XI("added neighbor: ", other_id, neighbor->node.comm);
      assert(neighbor->node.comm != MPI_COMM_NULL);
    }
    else
      free(neighbor);
  }
}

bool
KDA_Attach_to(KDA_Neighbor* neighbor, struct list* contacts)
{
  return list_add_unique(contacts, KDA_Neighbor_cmp, neighbor);
}

/**
   Form node->node connections
   Neighbor should already be in k_buckets, this does not do that
   @param node Referring node- not used in conn-A (no 3-way)
   @param neighbor The new neighbor
*/
void
KDA_Join(MPIRPC_Node node, KDA_Neighbor* neighbor)
{
  MPIRPC_Node other_node;
  MPIRPC_Node_make(MPI_COMM_WORLD, neighbor->node.rank, &other_node);
  char args[64];
  sprintf(args, "%X %i", id, mpi_rank);
  MPIRPC_Block(other_node, "join", args);
}

void
handle_join(MPIRPC_Node caller, int unique, char* args,
	    char* blob, int blob_length)
{
  SHOW_FI(caller.rank);
  KDA_ID other_id;
  int* other_rank = malloc(sizeof(int));
  sscanf(args, "%X %i", &other_id, other_rank);

  if (other_id != KDA_ID_CLIENT)
  {
    MPIRPC_Node node;
    MPIRPC_Node_make(caller.comm, caller.rank, &node);

    KDA_Neighbor neighbor;
    KDA_Neighbor_make_id(other_id, node, &neighbor);
    neighbor_add(&neighbor);
    hashtable_add(world_ranks, xheap(other_id), other_rank);
  }

  MPIRPC_Null(caller, unique);
}

MPIRPC_Handler(handle_get_rank)
{
  int* other_rank = (int*) hashtable_search(world_ranks, args);
  char* result = malloc(16*sizeof(char));
  int length = sprintf(result, "%i", *other_rank);
  MPIRPC_Return(caller, unique, result, length+1);
}

MPIRPC_Handler(handle_get_id)
{
  SHOW_FI(caller.rank);
  char* result = malloc(16*sizeof(char));
  int length = sprintf(result, "%X", id);
  MPIRPC_Return(caller, unique, result, length+1);
}

void
KDA_Detach(MPIRPC_Node node)
{}

void
KDA_Detach_hubs()
{}

KDA_Neighbor*
KDA_Neighbor_create_scan(char* data, int* n)
{
  NOTE_F;
  if (data == NULL)
    return NULL;
  int other_rank;
  KDA_ID other_id;
  char junk[64];
  char date[128];
  char time[64];
  int  count = sscanf(data, "[%X]=%i@%s %s %s %n",
                      &other_id, &other_rank, junk, date, time, n);

  if (count < 2)
    return NULL;
  KDA_Neighbor* nbor = malloc(sizeof(KDA_Neighbor));
  nbor->id   = other_id;
  nbor->node.rank = other_rank;
  nbor->node.comm = MPI_COMM_WORLD;
  struct tm t = {0};
  strcat(date, " ");
  strcat(date, time);
  SHOW(date);
  char* c = strptime(date, "%F %T", &t);
  assert(c);
  nbor->lastseen = mktime(&t);
  return nbor;
}
