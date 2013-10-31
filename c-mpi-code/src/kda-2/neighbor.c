
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "kda_neighbor-2.h"

static char output[100];

/**
   Create a single neighbor.  Uses KDA_Neighbor_create_scan().
*/
KDA_Neighbor*
KDA_Neighbor_create_string(char* info)
{
  int not_used;
  KDA_Neighbor* result = KDA_Neighbor_create_scan(info, &not_used);
  return result;
}

/**
   Glue a KDA_ID and a MPIRPC_Node together to make a neighbor.
*/
KDA_Neighbor*
KDA_Neighbor_create_id(KDA_ID other_id, MPIRPC_Node node)
{
  NOTE_F;
  KDA_Neighbor* result = malloc(sizeof(KDA_Neighbor));
  KDA_Neighbor_make_id(other_id, node, result);
  return result;
}

void
KDA_Neighbor_make_id(KDA_ID other_id, MPIRPC_Node node,
                     KDA_Neighbor* output)
{
   output->id       = other_id;
   output->node     = node;
   output->lastseen = time(NULL);
}

/**
   Copy the original into fresh storage
 */
KDA_Neighbor*
KDA_Neighbor_clone(KDA_Neighbor* original)
{
  KDA_Neighbor* clone =
      KDA_Neighbor_create_id(original->id, original->node);
  clone->lastseen = original->lastseen;
  return clone;
}

/**
   Scan the data for neighbors.
   @return The new neighbor.  Output n containing the number of chars
   consumed from data by sscanf().
*/


/**
   Neighbor-neighbor id comparison.
   @return
*/
int
KDA_Neighbor_cmp(void* object1, void* object2)
{
  KDA_Neighbor* n1 = (KDA_Neighbor*) object1;
  KDA_Neighbor* n2 = (KDA_Neighbor*) object2;

  if (n1->id > n2->id)
    return 1;
  if (n1->id < n2->id)
    return -1;
  return 0;
}

int
KDA_Neighbor_node_cmp(void* n1, void* n2)
{
  MPIRPC_Node node1 = ((KDA_Neighbor*) n1)->node;
  MPIRPC_Node node2 = *(MPIRPC_Node*) n2;
  if (node1.comm == node2.comm &&
      node1.rank == node2.rank)
    return 0;
  else
    if (node1.comm > node2.comm)
      return 1;
    else if (node1.comm < node2.comm)
      return -1;
    else
      if (node1.rank > node2.rank)
        return 1;
      else
        return -1;
}

/**
   Compare lastseen times for neighbors n1, n2.
   @return 1 iff n1 > n2, -1 iff n1 < n2, 0 iff n1 == n2
*/
int
KDA_Neighbor_time_cmp(void* object1, void* object2)
{
  KDA_Neighbor* n1 = (KDA_Neighbor*) object1;
  KDA_Neighbor* n2 = (KDA_Neighbor*) object2;

  if (n1->lastseen > n2->lastseen)
    return -1;
  else if (n1->lastseen < n2->lastseen)
    return 1;
  else
    return 0;
}

/**
   Writes string to static output location.
*/
char*
KDA_Neighbor_name(void* object)
{
  KDA_Neighbor_sprint(output, object);
  return output;
}

/**
   Name without lastseen.
*/
/*
int
KDA_Neighborname_sprint(char* buffer, void* object)
{
  KDA_Neighbor* n = (KDA_Neighbor*) object;
  return sprintf(buffer, "[%X]@ ", n->id);
}
*/

/**
   Writes string to static output location.
*/
char*
KDA_Neighbor_tostring(void* object)
{
  // NOTE_F;
  if (object == NULL)
    return NULL;
  KDA_Neighbor_sprint(output, object);
  return output;
}

/**
   Writes neighbor id to static output location.
*/
char*
KDA_Neighbor_id_tostring(void* object)
{
  if (object == NULL)
    return NULL;
  KDA_Neighbor* neighbor = (KDA_Neighbor*) object;
  sprintf(output, "%X", neighbor->id);
  return output;
}

int
KDA_Neighbor_sprint(char* buffer, void* object)
{
  // NOTE_F;
  int offset;
  KDA_Neighbor* neighbor = (KDA_Neighbor*) object;
  int  length;
  char name[100];

  // SHOW_FX(neighbor->id);

  if (neighbor->id == id)
  {
    offset   = sprintf(buffer, "[%X]=%i@SELF ", id, mpi_rank);
    time_t t = time(NULL);
    offset  += strftime(buffer+offset, 99, "%F %T",
                       localtime(&t));
  }
  else
  {
    assert(neighbor != NULL);

    if (neighbor->node.comm == MPI_COMM_NULL)
    {
      // NOTE("MPI_COMM_NULL");
      strcpy(name, "MPI_COMM_NULL");
    }
    else
    {
      // NOTE("MPI_Comm_get_name()");
      MPI_Comm_get_name(neighbor->node.comm, name, &length);
    }

    offset  = sprintf(buffer, "[%X]=%i@%s ", neighbor->id,
                      neighbor->node.rank, name);
    offset += strftime(buffer+offset, 99, "%F %T",
                       localtime(&neighbor->lastseen));
  }
  return offset;
}

void
KDA_Neighbor_dump(void* object)
{
  if (object == NULL)
    printf("(KDA_Neighbor NULL) \n");
  else
    printf("%s \n", KDA_Neighbor_tostring(object));
}

/**
   Scan the stream data for neighbors.
   Uses KDA_Neighbor_create_scan().
   @return A list of neighbors.
*/
struct list* KDA_Neighbor_node_list(char* data)
{
  SHOW_FS(data);

  struct list* result = list_create();
  char* end = data+strlen(data);
  while (data < end-3)
  {
    int n;
    KDA_Neighbor* neighbor = KDA_Neighbor_create_scan(data, &n);

    if (neighbor == NULL)
      break;
    data += n;
    list_add(result, neighbor);
    NOTE_X("neighbor: ", neighbor->id);
    // NOTE_II("scanned: ", other_id, other_rank);
  }

  return result;
}

