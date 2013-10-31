
#include <dense-1.h>
#include <cmpi_mode.h>

/**
   Number of service nodes.
*/
int dense_nodes;

/**
   When true, shutdown.
*/
bool dense_quitting = false;

/**
   ID for special sequential global operations such as
   debug dumps and shutdowns.
*/
int last_global_opid = 0;

/**
   Number of times buckets have been dumped.
   Useful for debugging only.
*/
int dumps = 0;

MPIRPC_Node* nodes;

int k = 1;

static MPIRPC_Handler(handle_bootping);
static MPIRPC_Handler(handle_info);
static MPIRPC_Handler(handle_ping);
static MPIRPC_Handler(handle_query_id);
static MPIRPC_Handler(handle_query_id_k);
static MPIRPC_Handler(handle_store);
static MPIRPC_Handler(handle_update);
static MPIRPC_Handler(handle_retrieve);
static MPIRPC_Handler(handle_debug_cache);
static MPIRPC_Handler(handle_quit);

void
DENSE_Init(int k_in, MPI_Comm comm)
{
  char hostname[100];

  NOTE_F;

  k = k_in;
  nodes = malloc(dense_nodes * sizeof(MPIRPC_Node));

  int i;
  for (i = 0; i < dense_nodes; i++)
    MPIRPC_Node_make(comm, i, &nodes[i]);
  MPIRPC_Register("bootping",    handle_bootping);
  MPIRPC_Register("info",        handle_info);
  MPIRPC_Register("ping",        handle_ping);
  MPIRPC_Register("query_id",    handle_query_id);
  MPIRPC_Register("query_id_k",  handle_query_id_k);
  MPIRPC_Register("store",       handle_store);
  MPIRPC_Register("update",      handle_update);
  MPIRPC_Register("retrieve",    handle_retrieve);
  MPIRPC_Register("debug_cache", handle_debug_cache);
  MPIRPC_Register("quit",        handle_quit);

  MPIRPC_Comm_add(MPI_COMM_WORLD);

  if (mpi_rank == 0)
    rpc_bootping();

  gethostname(hostname, 100);

  listen_loop();
}

void
DENSE_Init_client()
{
  MPIRPC_Comm_add(MPI_COMM_WORLD);
}

void
rpc_bootping()
{
  int next = cmpi_mode_next(mpi_rank, mpi_size, dense_nodes);
  NOTE_FI(next);
  if (next > 0)
  {
    MPIRPC_Node node;
    MPIRPC_Node_make(MPI_COMM_WORLD, next, &node);
    MPIRPC_Block(node, "bootping", NULL);
  }
  else
  {
    //int client =
    //  cmpi_mode_first_client(mpi_rank, mpi_size, dense_nodes);
    //NOTE("NOTIFYING");
    //int msg = -2;
    // MPI_Send(&msg, 1, MPI_INT, client, 0, MPI_COMM_WORLD);
  }
}

void
listen_loop()
{
  NOTE_F;

  while (! dense_quitting)
  {
    /*
    if (! MPIRPC_Check())
    {
      MPIRPC_Snooze();
    }
    else
    {
      MPIRPC_Snooze_reset();
    }
    */
    MPIRPC_Recv(MPI_COMM_WORLD);
  }
}

/**
   Query node at rank for key location.
*/
MPIRPC*
DENSE_Lookup(MPIRPC_Node node, char* key)
{
  SHOW_FIS(node.comm, key);

  DENSE_ID object_id = SHA1_mod(key);

  SHOW_X(object_id);

  return DENSE_Translate(node, object_id);
}

MPIRPC*
DENSE_Translate(MPIRPC_Node node, DENSE_ID id)
{
  char args[16];
  sprintf(args, "%i", id);
  MPIRPC* rpc =
    MPIRPC_Call(node, "query_id", args,
                NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

/**
   Query node at rank for k closest key locations.
*/
MPIRPC*
DENSE_Lookup_k(MPIRPC_Node node, char* key)
{
  DENSE_ID object_id = SHA1_mod(key);
  SHOW_FISX(node.rank, key, object_id);

  return DENSE_Translate_k(node, object_id);
}

MPIRPC*
DENSE_Translate_k(MPIRPC_Node node, DENSE_ID id)
{
  char args[32];
  sprintf(args, "%X", id);
  MPIRPC* rpc =
    MPIRPC_Call(node, "query_id_k", args, NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

/**
   Instruct a node to store a value.
   Asynchronous.  Copies key into MPIRPC.
*/
MPIRPC*
DENSE_Store(MPIRPC_Node node, char* key, char* value, int length)
{
  SHOW_FSI(key, length);

  MPIRPC* rpc = MPIRPC_Call_blob(node, "store", key,
                                 value, length,
                                 NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

/**
   Frees value on completion
*/
MPIRPC*
DENSE_Update(MPIRPC_Node node, char* key, char* value,
             int length, int offset)
{
  SHOW_FSI(key, length);

  char args[MPIRPC_MAX_ARGS];
  sprintf(args, "%s %i", key, offset);

  MPIRPC* rpc =
      MPIRPC_Call_blob(node, "update", args, value, length,
                       NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

/**
   Instruct a node to retrieve a value.
   Asynchronous.  Copies key into MPIRPC.
   User must use MPIRPC->result_length
                to find number of bytes retrieved.
*/
MPIRPC*
DENSE_Retrieve(MPIRPC_Node node, char* key)
{
  MPIRPC* rpc =
    MPIRPC_Call(node, "retrieve", key, NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

void
DENSE_Debug_cache(MPIRPC_Node node)
{
  MPIRPC_Block(node, "debug_cache", NULL);
}

void
DENSE_Shutdown(MPIRPC_Node node)
{
  NOTE_F;
  MPIRPC_Block(node, "quit", NULL);
}

static MPIRPC_Handler(handle_bootping)
{
  MPIRPC_Null(caller, unique);
  rpc_bootping();
}

static MPIRPC_Handler(handle_info)
{
  MPIRPC_Null(caller, unique);
}

static MPIRPC_Handler(handle_ping)
{
  MPIRPC_Null(caller, unique);
}

static MPIRPC_Handler(handle_query_id)
{
  int object_id;
  sscanf(args, "%X", &object_id);
  int   other_rank = object_id % dense_nodes;
  char* result = malloc(10*sizeof(char));
  int length = sprintf(result, "%i", other_rank)+1;
  MPIRPC_Return(caller, unique, result, length);
}

static MPIRPC_Handler(handle_query_id_k)
{
  int i;
  int object_id;
  sscanf(args, "%X", &object_id);
  struct inlist* ranks = inlist_create();
  for (i = 0; i < k; i++)
  {
    int other_rank = (object_id+i) % dense_nodes;
    inlist_add(ranks, other_rank);
  }
  char* result = inlist_serialize(ranks);
  inlist_free(ranks);
  MPIRPC_Return(caller, unique, result, strlen(result)+1);
}

static MPIRPC_Handler(handle_store)
{
  SHOW_FSI(args, blob_length);

  char* key = heap(args);
  cmpi_cached_store(key, blob, blob_length);
  DEBUG(lru_table_printdata(cmpi_cache));

  MPIRPC_Null(caller, unique);
  DONE;
}

static MPIRPC_Handler(handle_update)
{
  char* key = malloc(strlen(args)+1);

  SHOW_FSI(args, blob_length);

  int offset;
  int n = sscanf(args, "%s %i", key, &offset);
  assert(n == 2);

  cmpi_cached_update(key, blob, offset, blob_length);
  // DEBUG(lru_table_printdata("%s", cmpi_cache));

  MPIRPC_Null(caller, unique);
}

static MPIRPC_Handler(handle_retrieve)
{
  NOTE_FS(args);

  char* data;
  int   length = cmpi_cached_retrieve(args, &data);

  // printdata("result1: ", data, length);

  char* result = malloc(CMPI_VALUE_LENGTH);

  SHOW_P(data);

  memcpy(result, data, length);

  if (data)
  {
    MPIRPC_Return(caller, unique, result, length);
  }
  else
  {
    NOTE("handle_retrieve() returning NULL");
    MPIRPC_Null(caller, unique);
  }
}

static MPIRPC_Handler(handle_debug_cache)
{
  MPIRPC_Null(caller, unique);
  puts("cmpi_cache:");
  lru_table_printdata(cmpi_cache);
}

static MPIRPC_Handler(handle_quit)
{
  NOTE_F;
  MPIRPC_Null(caller, unique);
  sleep(1);
  MPIRPC_Flush_returns();
  dense_quitting = true;
  // TODO: cleanup
}
