
#include <cmpi_mode.h>

#include <cmpi_dense-1.h>

#include <dense-1.h>

/**
   List of node ranks to contact for DENSE routines.
*/
static struct inlist* contacts;

// void add_contacts(void);

static void DENSE_Read_params(void);

CMPI_RETURN
cmpi_init_impl()
{
  // NOTE_F;

  gossip_debug(MASK_CMPI, "node: %i\n", mpi_rank);

  DENSE_Read_params();

  DENSE_Init(1, 3);

  return CMPI_SUCCESS;
}

CMPI_RETURN
cmpi_init_impl_client()
{
  gossip_debug(MASK_CMPI, "client: %i\n", mpi_rank);

  DENSE_Read_params();

  DENSE_Init_client();

  contacts = cmpi_mode_contacts(mpi_rank, mpi_size, dense_nodes);

  return CMPI_SUCCESS;
}

CMPI_RETURN
cmpi_impl_parameter(char* name, char* value)
{
  if (strcmp(name, "nodes") == 0)
  {
    int n;
    sscanf(value, "%i", &n);
    dense_nodes = n;
  }

  return CMPI_SUCCESS;
}

char*
cmpi_info(CMPI_ID key)
{
  char* result = NULL;
  int   i      = inlist_random(contacts);

  MPIRPC_Node node;
  MPIRPC_Node_make(i, MPI_COMM_WORLD, &node);

  result = MPIRPC_Block(node, "info", xheap(key));

  return result;
}

/**
   Ex-DHT search.
   @return The MPI rank closest to the given key.
*/
CMPI_ID
cmpi_lookup(char* key)
{
  int   i      = inlist_random(contacts);

  NOTE_FS(key);

  i = inlist_random(contacts);

  MPIRPC_Node node;
  MPIRPC_Node_make(i, MPI_COMM_WORLD, &node);

  MPIRPC* rpc = DENSE_Lookup(node, key);
  MPIRPC_Wait(rpc);

  // printf("cmpi_lookup()@%i -> %s \n", i, rpc->result);

  return 0;
}

CMPI_ID
cmpi_translate(CMPI_ID id)
{
  NOTE_F;

  int i = inlist_random(contacts);
  MPIRPC_Node node;
  MPIRPC_Node_make(MPI_COMM_WORLD, i, &node);

  MPIRPC* rpc = DENSE_Translate(node, (DENSE_ID) id);
  MPIRPC_Wait(rpc);
  int result = 0;
  sscanf(rpc->result, "%X", &result);
  return result;
}

/**
   Blocking call.
*/
CMPI_RETURN
//cmpi_put(char* key, char* value, int length)
cmpi_put(char* key, void* value, int length)
{
  struct inlist_item* item;
  MPIRPC_Node node;
  SHOW_FS(key);

  int i = inlist_random(contacts);
  MPIRPC_Node_make(MPI_COMM_WORLD, i, &node);
  MPIRPC* lookup = DENSE_Lookup_k(node, key);
  MPIRPC_Wait(lookup);

  struct inlist* ranks = inlist_parse(lookup->result);
  free(lookup->result);
  MPIRPC_Free(lookup);

  NOTE("got ranks: ");
  DEBUG(inlist_printf(ranks));

  for (item = ranks->head; item; item = item->next)
  {
    SHOW_I(item->data);
    MPIRPC_Node_make(MPI_COMM_WORLD, item->data, &node);
    MPIRPC* rpc = DENSE_Store(node, key, value, length);
    MPIRPC_Wait(rpc);
    MPIRPC_Free(rpc);
  }
  inlist_free(ranks);

  DONE;
  return CMPI_SUCCESS;
}

/**
   Blocking call.
*/
CMPI_RETURN
//cmpi_update(char* key, char* value, int length, int offset)
cmpi_update(char* key, void* value, int length, int offset)
{
  struct inlist_item* item;
  MPIRPC_Node node;
  SHOW_FS(key);

  int i = inlist_random(contacts);
  MPIRPC_Node_make(MPI_COMM_WORLD, i, &node);
  MPIRPC* lookup = DENSE_Lookup_k(node, key);
  MPIRPC_Wait(lookup);

  struct inlist* ranks = inlist_parse(lookup->result);

  for (item = ranks->head;
       item; item = item->next)
  {
    SHOW_I(item->data);
    MPIRPC_Node_make(MPI_COMM_WORLD, item->data, &node);
    MPIRPC* rpc = DENSE_Update(node, key, value, length, offset);
    MPIRPC_Wait(rpc);
    MPIRPC_Free(rpc);
  }
  inlist_free(ranks);

  DONE;
  return CMPI_SUCCESS;
}

/**
   Blocking call.
*/
CMPI_RETURN
//cmpi_get(char* key, char** value, int* length)
cmpi_get(char* key, void** value, int* length)
{
  int i = inlist_random(contacts);
  MPIRPC_Node node;

  NOTE_FS(key);

  MPIRPC_Node_make(MPI_COMM_WORLD, i, &node);

  MPIRPC* lookup = DENSE_Lookup_k(node, key);
  MPIRPC_Wait(lookup);

  struct inlist* ranks = inlist_parse(lookup->result);
  int rank = inlist_random(ranks);
  inlist_free(ranks);
  free(lookup->result);
  MPIRPC_Free(lookup);

  MPIRPC_Node_make(MPI_COMM_WORLD, rank, &node);
  MPIRPC* rpc = DENSE_Retrieve(node, key);
  MPIRPC_Wait(rpc);
  *value  = rpc->result;
  *length = rpc->result_length;

  SHOW_I(rpc->result_length);

  MPIRPC_Free(rpc);

  DONE;
  return CMPI_SUCCESS;
}

void
cmpi_shutdown()
{
  NOTE_F;

  struct inlist_item* item;
  struct inlist* list =
    cmpi_shutdown_list(mpi_rank, mpi_size, cmpi_nodes);

  MPIRPC_Node node;
  for (item = list->head; item; item = item->next)
  {
    MPIRPC_Node_make(MPI_COMM_WORLD, item->data, &node);
    DENSE_Shutdown(node);
  }
  inlist_free(list);
  DONE;
}

void
cmpi_debug_tables()
{}

void
cmpi_debug_caches()
{
  struct inlist_item* item;
  MPIRPC_Node node;
  for (item = contacts->head; item; item = item->next)
  {
    MPIRPC_Node_make(MPI_COMM_WORLD, item->data, &node);
    DENSE_Debug_cache(node);
  }
}

/**
   Extract parameters from C-MPI specific to DENSE
 */
static void
DENSE_Read_params(void)
{
  // NOTE_F;
  char* nodes_msg = cmpi_params_get("nodes");
  if (nodes_msg)
  {
    int n = sscanf(nodes_msg, "%i", &dense_nodes);
    // SHOW_I(dense_nodes);
    assert(n == 1);
  }
  else
  {
    dense_nodes = mpi_size;
  }
}
