
#include <kda-2.h>

#include <strings.h>

/**
   Uniquify KDA_Operations
 */
static int unique = 0;

KDA_ID   id;
K_BUCKET k_bucket[KDA_SPACE_SIZE];

/**
   Number of nodes from MPI_COMM_WORLD that will be DHT nodes.
*/
int kda_nodes;

/**
   List of KDA_Neighbor clients.
*/
struct list* clients;

MPI_Request request;

/**
   Outstanding KDA_Operations:
*/
struct itable* operations;

/**
   Kademlia parameter alpha:
*/
int alpha;

/**
   Kademlia parameter k:
*/
int k;

/**
   A neighbor pointer to myself.
*/
KDA_Neighbor* self;

/**
   My port name.
*/
char port[MPI_MAX_PORT_NAME];

/**
   A uniquifier for client numbers.  (Debugging only).
*/
int unique_client = 1;

/**
   Use node to connect to neighbor.
*/
KDA_Neighbor* check_neighbor(MPIRPC_Node node, KDA_Neighbor* neighbor);


//// Neighbor management...

/**
   Add neighbor to the neighbor table.
   @return false iff n is already in the neighbor table.
*/
bool neighbor_add(KDA_Neighbor* neighbor);

/**
   Add neighbor to the client list.
*/
void client_add(KDA_Neighbor* client);

/**
   Find neighbor in buckets and set its comm.
*/
void neighbor_set_node(KDA_Neighbor* neighbor, MPIRPC_Node node);

/**
   Translate a dummy neighbor that is set to MPI_COMM_NULL to
   a good one in a bucket.
   TODO: Speed this up with an index or something.
*/
KDA_Neighbor* neighbor_lookup(KDA_Neighbor* neighbor);

void   dump_bucket_ids(void);
char*  bucket_ids_tostring(void);
void   dump_buckets(void);
char*  buckets_tostring(void);
char*  kda_op_tostring(KDA_Operation* op);

static KDA_ID make_id(int salt);
// static char*  id_tostring(void);
// static void   bootstrap(MPIRPC_Node node);

int KDA_Neighbor_table_size(void);

//// Handlers...

/**
   Generate info string for debugging
*/
void handle_info(MPIRPC_Node node, int unique, char* args,
                 void* blob, int blob_length);
void handle_join(MPIRPC_Node caller, int unique, char* args,
                 void* blob, int blob_length);
void handle_link(MPIRPC_Node caller, int unique, char* args,
                 void* blob, int blob_length);
void handle_find_node(MPIRPC_Node node, int unique, char* args,
                      void* blob, int blob_length);
void handle_neighbor(MPIRPC_Node node, int unique, char* args,
                     void* blob, int blob_length);
void handle_ping(MPIRPC_Node node, int unique, char* args,
                 void* blob, int blob_length);
void handle_query_id(MPIRPC_Node node, int unique, char* args,
                     void* blob, int blob_length);
void handle_query_id_k(MPIRPC_Node node, int unique, char* args,
                       void* blob, int blob_length);
void handle_store(MPIRPC_Node node, int unique, char* args,
                  void* blob, int blob_length);
void handle_update(MPIRPC_Node node, int unique, char* args,
		   void* blob, int blob_length);
void handle_retrieve(MPIRPC_Node node, int unique, char* args,
                     void* blob, int blob_length);
void handle_shutdown(MPIRPC_Node caller, int unique, char* args,
                     void* blob, int blob_length);

// Synchronous methods...
struct ilist* find_node(KDA_ID id);

// Asynchronous methods and return services...
void query_id(MPIRPC_Node node, int unique, KDA_ID id);
void return_query_id(KDA_Operation* op);
void query_id_k(MPIRPC_Node node, int unique, KDA_ID id);
void return_query_id_k(KDA_Operation* op);

// RPCs...

void rpc_find_node(KDA_Neighbor* neighbor, KDA_Operation* op);

// Proceeds...
void proceed_find(MPIRPC* rpc);
void proceed_ping(MPIRPC* rpc);
void proceed_link(MPIRPC* rpc);

void
KDA_Data(int alpha_in, int k_in)
{
  NOTE_F;

  alpha = alpha_in;
  k     = k_in;

  for (int i = 0; i < KDA_SPACE_SIZE; i++)
    k_bucket[i] = list_create();

  operations = itable_create(CALL_TABLE_SIZE);
}

/**
   Setup Kademlia.
   Must be called after MPI_Init() and whoami().
*/
void
KDA_Init(int alpha_in, int k_in)
{
  NOTE_F;

  KDA_Data(alpha_in, k_in);

  MPIRPC_Node node;
  node.comm = MPI_COMM_NULL;
  node.rank = 0;
  id      = make_id(debug_rank);
  self    = KDA_Neighbor_create_id(id, node);
  NOTE_XI("SELF: ", id, id);

  port[0] = '\0';

  clients = list_create();

  MPIRPC_Register("info",       handle_info);
  // MPIRPC_Register("neighbor",   handle_neighbor);
  MPIRPC_Register("ping",       handle_ping);
  MPIRPC_Register("join",       handle_join);
  MPIRPC_Register("find_node",  handle_find_node);
  MPIRPC_Register("query_id",   handle_query_id);
  MPIRPC_Register("query_id_k", handle_query_id_k);
  MPIRPC_Register("store",      handle_store);
  MPIRPC_Register("retrieve",   handle_retrieve);
  MPIRPC_Register("shutdown",   handle_shutdown);

  KDA_Init_conn();
}

void
KDA_Init_client(int alpha_in, int k_in)
{
  NOTE_F;
  id = KDA_ID_CLIENT;

  KDA_Data(alpha_in, k_in);
}

KDA_Neighbor*
KDA_Random_neighbor()
{
  int i;
  struct list_item* item;
  // NOTE_F;

  struct list* neighbors = list_create();
  for (i = 0; i < KDA_SPACE_SIZE; i++)
  {
    for (item = k_bucket[i]->head;
         item; item = item->next)
    {
      KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
      if (neighbor->node.comm != MPI_COMM_NULL)
        list_add(neighbors, neighbor);
    }
  }
  // NOTE_I("neighbors for random: ", neighbors->size);
  KDA_Neighbor* result = list_random(neighbors);
  list_free(neighbors);
  return result;
}

void
neighbor_set_node(KDA_Neighbor* neighbor, MPIRPC_Node node)
{
  int i;
  struct list_item* item;
  NOTE_FX(neighbor->id);
  for (i = 0; i < KDA_SPACE_SIZE; i++)
  {
    for (item = k_bucket[i]->head;
         item; item = item->next)
    {
      KDA_Neighbor* n = (KDA_Neighbor*) item->data;
      if (n->id == neighbor->id)
      {
        n->node = node;
        KDA_Comm_set_name(n);
      }
    }
  }
}

/**
   Use DHT to synchronously find the given key.
*/
struct ilist*
KDA_Find_key(char* key)
{
  NOTE_F;

  int id = hash_string(key, KDA_HASH_SPACE);
  return KDA_Find(id);
}

/**
   Use DHT to synchronously find the given key.
   @return An ilist of neighbors (op->k_closest).
*/
struct ilist*
KDA_Find(KDA_ID id)
{
  KDA_Operation* op = KDA_Operate(id, NULL, NULL, NULL, NULL);
  KDA_Wait(op);
  return op->k_closest;
}

void
KDA_Wait(KDA_Operation* op)
{
  MPIRPC_Snooze_reset();
  while (op->status != KDA_STATUS_COMPLETE)
  {
    if (MPIRPC_Check())
      MPIRPC_Snooze_reset();
    else
      MPIRPC_Snooze();
  }
}

/**
   Ordered local closest node lookup
   @return k neighbors in a list
*/
struct ilist*
KDA_Closest(KDA_ID object_id)
{
  NOTE_FX(object_id);

  struct ilist* result = ilist_create();

  int d = XOR(self->id, object_id);
  ilist_add(result, d, self);

  for (int i = 0; i < KDA_SPACE_SIZE; i++)
  {
    for (struct list_item* item = k_bucket[i]->head;
         item; item = item->next)
    {
      KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
      d = XOR(neighbor->id, object_id);
      if (result->size < k)
      {
        ilist_ordered_insert(result, d, neighbor);
      }
      else if (d < result->tail->key)
      {
        ilist_ordered_insert(result, d, neighbor);
        ilist_pop(result);
      }
    }
  }
  return result;
}

/**
   Could fail if other_id is not in local neighbor table.
*/
KDA_Neighbor*
KDA_Neighbor_ID(KDA_ID other_id)
{
  NOTE_FX(other_id);

  KDA_Neighbor* result = NULL;

  if (other_id == id)
    return self;

  struct ilist* k_closest = KDA_Closest(other_id);

  result = ilist_search(k_closest, 0);
  return result;
}

/**
   Return the KDA_Neighbor for this MPIRPC_Node.
   Could fail if other_id is not in local neighbor table.
 */
KDA_Neighbor*
KDA_Neighbor_node(MPIRPC_Node node)
{
  int i;
  struct list_item* item;
  NOTE_F;

  for (i = 0; i < KDA_SPACE_SIZE; i++)
  {
    for (item = k_bucket[i]->head;
         item; item = item->next)
    {
      KDA_Neighbor* n = (KDA_Neighbor*) item->data;
      if (node.comm == n->node.comm)
        return n;
    }
  }

  for (item = clients->head;
       item; item = item->next)
  {
    KDA_Neighbor* n = (KDA_Neighbor*) item->data;
    if (node.comm == n->node.comm)
      return n;
  }

  NOTE("KDA_Neighbor_node: NULL!");
  return NULL;
}

/**
   Query node for key location.
*/
MPIRPC*
KDA_Lookup(MPIRPC_Node node, char* key)
{
  NOTE_F;

  KDA_ID object_id = hash_string(key, KDA_HASH_SPACE);

  printf("id: %i \n", object_id);

  return KDA_Translate(node, object_id);
}

/**
   Query node for k closest key locations.
*/
MPIRPC*
KDA_Lookup_k(MPIRPC_Node node, char* key)
{
  KDA_ID object_id = SHA1_mod(key);
  // NOTE_X("id: ", object_id);

  return KDA_Translate_k(node, object_id);
}

/**
   Query node for k closest key locations.
*/
MPIRPC*
KDA_Translate_k(MPIRPC_Node node, KDA_ID id)
{
  NOTE_FX(id);
  char sid[32];
  sprintf(sid, "%X", id);
  MPIRPC* rpc =
    MPIRPC_Call(node, "query_id_k", sid, NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

/**
   Query node at rank for key location.
*/
MPIRPC*
KDA_Translate(MPIRPC_Node node, KDA_ID id)
{
  NOTE_F;

  return MPIRPC_Call(node, "query_id", xheap(id), NULL, NULL);
}

/**
   Instruct a node to store a value.  Asynchronous.
   @return MPIRPC pointer for asynchronous call.
*/
MPIRPC*
KDA_Store(KDA_Neighbor* neighbor, char* key, char* value, int length)
{
  NOTE_F;

  return MPIRPC_Call_blob(neighbor->node, "store", key,
                          value, length,
                          NULL, MPIRPC_PROCEED_NULL);
}

/**
   Instruct a node to retrieve a value.  Asynchronous.
   @return MPIRPC pointer for asynchronous call.
*/
MPIRPC*
KDA_Retrieve(KDA_Neighbor* neighbor, char* key)
{
  NOTE_F;
  return MPIRPC_Call(neighbor->node, "retrieve", key,
                     NULL, MPIRPC_PROCEED_NULL);
}

MPIRPC*
KDA_Update(KDA_Neighbor* neighbor, char* key, char* value,
	   int length, int offset)
{
  SHOW_FSI(key, length);

  char args[MPIRPC_MAX_ARGS];
  sprintf(args, "%s %i", key, offset);

  MPIRPC* rpc =
    MPIRPC_Call_blob(neighbor->node, "update", args,
		     value, length, NULL, MPIRPC_PROCEED_NULL);
  return rpc;
}

KDA_Query*
query_create(MPIRPC_Node caller, int unique,
             void (*service)(KDA_Operation* op))
{
  NOTE_F;
  KDA_Query* query = (KDA_Query*) malloc(sizeof(KDA_Query));
  query->caller  = caller;
  query->unique  = unique++;
  query->service = service;
  return query;
}

void
KDA_Comm_set_name(KDA_Neighbor* neighbor)
{
  NOTE_FX(neighbor->id);
  char name[32];
  sprintf(name, "%X", neighbor->id);
  MPI_Comm_set_name(neighbor->node.comm, name);
}

void
handle_info(MPIRPC_Node caller, int unique, char* args,
            void* blob, int blob_length)
{
  // NOTE_F;
  char* result = buckets_tostring();
  MPIRPC_Return(caller, unique, result, strlen(result)+1);
}

/**
    Store key/value pair.
    @param args The key
    @param blob The value
    @return NULL
*/
void
handle_store(MPIRPC_Node caller, int unique, char* args,
             void* blob, int blob_length)
{
  NOTE_S("store: ", args);
  char* key = strdup(args);
  cmpi_cached_store(key, blob, blob_length);
  DEBUG(lru_table_printf("%s", cmpi_cache));
  MPIRPC_Null(caller, unique);
}

void
handle_update(MPIRPC_Node caller, int unique, char* args,
             void* blob, int blob_length)
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

/**
   Obtain value given key.
   @param args The key
   @return Pointer to value (not thread-safe on modification!)
*/
void
handle_retrieve(MPIRPC_Node caller, int unique, char* args,
                void* blob, int blob_length)
{
  NOTE_FS(args);
  DEBUG(lru_table_printf("%s", cmpi_cache));

  void* data;
  int length;
  length = cmpi_cached_retrieve(args, &data);
  void* copy = malloc(length);
  memcpy(copy, data, length);

  SHOW_S(copy);
  MPIRPC_Return(caller, unique, copy, length);
}

void
handle_shutdown(MPIRPC_Node caller, int unique, char* args,
                void* blob, int blob_length)
{
  NOTE_F;
  cmpi_quitting = true;
  MPIRPC_Null(caller, unique);
}

void
rpc_find_node(KDA_Neighbor* neighbor, KDA_Operation* op)
{
  NOTE_FXX(neighbor->id, op->object_id);
  char xid[32];
  sprintf(xid, "%X", op->object_id);
  MPIRPC_Call(neighbor->node, "find_node", xid, op, proceed_find);
}

void
handle_find_node(MPIRPC_Node caller, int unique, char* args,
                 void* blob, int blob_length)
{
  ;
  NOTE_FS(args);
  // Decode...
  int object_id;
  sscanf(args, "%X", &object_id);

  // Search...
  struct ilist* known = KDA_Closest(object_id);

  // Encode...
  char* result = (char*) malloc(1024*sizeof(char));
  char* s = result;
  s += sprintf(s, "%i ", known->size);
  for (struct ilist_item* item = known->head; item;
       item = item->next)
    s += sprintf(s, "%s ", KDA_Neighbor_name(item->data));
  ilist_free(known);

  MPIRPC_Return(caller, unique, result, strlen(result)+1);
}

void
handle_query_id(MPIRPC_Node caller, int unique, char* args,
                void* blob, int blob_length)
{
  int object_id;
  sscanf(args, "%X", &object_id);
  query_id(caller, unique, object_id);
}

void
query_id(MPIRPC_Node caller, int unique, KDA_ID object_id)
{
  NOTE_FX(object_id);
  KDA_Operate(object_id, NULL, NULL, NULL,
              query_create(caller, unique, return_query_id));
}

void
return_query_id(KDA_Operation* op)
{
  KDA_Neighbor* n = (KDA_Neighbor*) ilist_poll(op->k_closest);
  char* result = xheap(n->id);
  MPIRPC_Return(op->query->caller, op->query->unique,
                result, strlen(result)+1);
  // free op
}

void
handle_query_id_k(MPIRPC_Node caller, int unique, char* args,
                  void* blob, int blob_length)
{
  int object_id;
  sscanf(args, "%X", &object_id);
  NOTE_S("args: ", args);
  // NOTE_X("oid: ", object_id);
  query_id_k(caller, unique, object_id);
}

void
query_id_k(MPIRPC_Node caller, int unique, KDA_ID object_id)
{
  NOTE_F;
  // NOTE_X("oid: ", object_id);
  KDA_Operate(object_id, NULL, NULL, NULL,
              query_create(caller, unique, return_query_id_k));
}

void
return_query_id_k(KDA_Operation* op)
{
  NOTE_F;
  char* result = malloc(2048*sizeof(char));
  char* p = result;
  while (op->k_closest->size > 0)
  {
    KDA_Neighbor* n = (KDA_Neighbor*) ilist_poll(op->k_closest);
    p += KDA_Neighbor_sprint(p, n);
    if (op->k_closest->size > 0)
      p += sprintf(p, " ");
  }

  SHOW_S(result);
  MPIRPC_Return(op->query->caller, op->query->unique,
                result, strlen(result)+1);
  // free op
}

/** Perform op->name on each node of op->k_closest
    or carry out op->service().
*/
void
KDA_Map(KDA_Operation* op)
{
  NOTE_F;
  if (op->name)
  {
    printf("map: %s \n", op->name);
    for (struct ilist_item* item = op->k_closest->head; item;
         item = item->next)
    {
      KDA_Neighbor* n = (KDA_Neighbor*) item->data;
      if (n->id == id)
        continue;
      MPIRPC_Call(n->node, op->name, op->args,
                  op, op->proceed);
    }
  }
  else if (op->query)
  {
    NOTE("op->query");
    op->query->service(op);
  }
  else assert(false);

  op->returned = list_create();
}

void
proceed_find(MPIRPC* rpc)
{
  NOTE_F;
  KDA_Operation* op = (KDA_Operation*) rpc->extras;

  list_remove_where(op->outstanding, KDA_Neighbor_node_cmp,
                    &rpc->target);

  // NOTE("from: ");
  // NOTE(MPIRPC_Comm_get_name(rpc->target.comm));

  NOTE("outstanding: ");
  DEBUG(list_output(KDA_Neighbor_tostring, op->outstanding));

  int size;
  int n;
  char* p = rpc->result;
  sscanf(rpc->result, "%i %n", &size, &n);
  p += n;

  struct list* neighbors = KDA_Neighbor_node_list(p);
  for (struct list_item* item = neighbors->head; item;
       item = item->next)
  {
    KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
    neighbor = check_neighbor(rpc->target, neighbor);
    if (neighbor == NULL)
      continue;

    int d = XOR(neighbor->id, op->object_id);
    if (d < op->k_closest->tail->key);
    {
      NOTE("improved");
      op->improved = true;
      // printf("inserting: %s \n", KDA_Neighbor_name(n));
      ilist_ordered_insert_unique(op->k_closest, KDA_Neighbor_cmp,
                                  d, neighbor);
      // ilist_output(KDA_Neighbor_tostring, op->k_closest);
      if (op->k_closest->size > k)
        ilist_pop(op->k_closest);
    }
  }
  list_destroy(neighbors);

  free(rpc->result);
  MPIRPC_Free(rpc);

  if (op->outstanding->size == 0)
  {
    if (op->improved)
    {
      if (! KDA_Find_node_again(op))
        KDA_Map(op);
      else
        NOTE("Doing another round.");
    }
    else
      KDA_Map(op);
  }
  NOTE_I("outstanding: ", op->outstanding->size);
  NOTE_I("contacted:   ", op->contacted->size);
  NOTE("proceed_find done");
}

/**
   Translate a dummy neighbor that is set to MPI_COMM_NULL to
   a good one in a bucket.
   TODO: Speed this up with an index or something.
   @return neighbor from bucket.  NULL if given self id.
*/
KDA_Neighbor*
neighbor_lookup(KDA_Neighbor* neighbor)
{
  NOTE_F;
  if (neighbor->id == id)
    return NULL;

  for (int i = 0; i < KDA_SPACE_SIZE; i++)
    for (struct list_item* item = k_bucket[i]->head; item;
         item = item->next)
    {
      KDA_Neighbor* n = (KDA_Neighbor*) item->data;
      if (n->id == neighbor->id)
        return n;
    }
  NOTE("LOOKUP FAILED!");
  return NULL;
}

/**
   If this neighbor is unknown, use node to connect to neighbor.
   @return neighbor from bucket.  NULL if given self id.
*/
KDA_Neighbor*
check_neighbor(MPIRPC_Node node, KDA_Neighbor* neighbor)
{
  NOTE_FX(neighbor->id);
  KDA_Neighbor* result;

  if (neighbor_add(neighbor))
    KDA_Join(node, neighbor);

  result = neighbor_lookup(neighbor);
  return result;
}

/**
   Object location method.  Asynchronous.
*/
bool
KDA_Find_node(KDA_Operation* op)
{
  NOTE_F;

  op->k_closest = KDA_Closest(op->object_id);

  DEBUG(printf("local closest: \n");
        ilist_dump(KDA_Neighbor_id_tostring, op->k_closest););

  return KDA_Find_node_again(op);
}

/**
   Recursive object location method.
*/
bool
KDA_Find_node_again(KDA_Operation* op)
{
  struct ilist_item* item;
  NOTE_F;

  // True iff we actually issued another RPC.
  bool contacted = false;
  op->improved   = false;

  int i = 0;
  for (item = op->k_closest->head;
       (i < alpha) && item; item = item->next)
  {
    KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
    if (neighbor->id == id)
      continue;
    if (! list_contains(op->contacted, KDA_Neighbor_cmp, neighbor))
    {
      list_add(op->contacted,   neighbor);
      list_add(op->outstanding, neighbor);
      rpc_find_node(neighbor, op);
      NOTE_I("contacted: ", op->contacted->size);
      contacted = true;
      i++;
    }
  }
  return contacted;
}

/**
   Locate the object_id and call its host method and args.
*/
KDA_Operation*
KDA_Operate(KDA_ID object_id, char* name, void* args,
            void (*proceed)(MPIRPC* rpc), KDA_Query* query)
{
  KDA_Operation* op =
    (KDA_Operation*) malloc(sizeof(KDA_Operation));

  op->object_id   = object_id;
  op->id          = unique++;
  op->name        = name;
  op->args        = args;
  op->k_closest   = NULL;
  op->contacted   = list_create();
  op->outstanding = list_create();
  op->improved    = false;
  op->status      = KDA_STATUS_SEARCHING;
  op->proceed     = proceed;
  op->query       = query;

  itable_add(operations, op->id, op);

  KDA_Find_node(op);
  return op;
}

void
KDA_Operation_free(KDA_Operation* op)
{
  list_free(op->contacted);
  list_free(op->outstanding);
  ilist_free(op->k_closest);
  itable_remove(operations, op->id);
  free(op);
}

void
KDA_Ping(KDA_ID other_id)
{
  NOTE_FX(other_id);
  KDA_Operate(other_id, "ping", NULL, proceed_ping, NULL);
}

void
proceed_ping(MPIRPC* rpc)
{
  NOTE_F;

  MPIRPC_Node* target = malloc(sizeof(MPIRPC_Node));
  *target = rpc->target;

  KDA_Operation* op = (KDA_Operation*) rpc->extras;
  list_add(op->returned, target);

  DEBUG(dump_buckets());

  // TODO: Deallocate op and rpc
  // if (mpi_rank == mpi_size-1) NOTE("Highest rank ping completed.");
  if (cmpi_status == CMPI_STATUS_PROTO)
  {
    char output[20];
    cmpi_status = CMPI_STATUS_READY;
    sprintf(output, "%i", mpi_rank);
    timestamp("READY", output);
  }
}

/**
   Manufacture a random ID.
*/
KDA_ID
make_id(int salt)
{
  srand(mpi_rank+4+salt);
  KDA_ID result = 0;
  while (result == 0 || result == 1)
    result = rand();
  return result;
}

/**
   @return True if copy of n is added to the neighbor table -
           use neighbor_lookup to obtain this copy after a KDA_Join
*/
bool
k_bucket_insert(int i, KDA_Neighbor* n)
{
  // NOTE_FI(i);

  if (list_contains(k_bucket[i], KDA_Neighbor_cmp, n))
  {
    NOTE_X("I already have: ", n->id);
    return false;
  }

  KDA_Neighbor* copy = KDA_Neighbor_clone(n);
  list_ordered_insert(k_bucket[i], KDA_Neighbor_time_cmp, copy);

  NOTE("After insert: ");
  DEBUG(dump_buckets());

  return true;
}

/**
   @return True if copy of neighbor is added to the neighbor table.
*/
bool
neighbor_add(KDA_Neighbor* neighbor)
{
  NOTE_FX(neighbor->id);

  // Useful in bootstrap cases:
  if (neighbor == NULL)
    return false;

  // Do not insert self.  Would screw up KDA_Closest().
  if (id == neighbor->id)
    return false;

  int d = XOR(id, neighbor->id);
  int i = ilog2(d);

  return k_bucket_insert(i, neighbor);
}

void
client_add(KDA_Neighbor* client)
{
  list_add(clients, client);
}

/**
   MPIRPC registered function.
   No native method.  Simply return.
*/
void
handle_ping(MPIRPC_Node caller, int unique, char* args,
            void* blob, int blob_length)
{
  MPIRPC_Null(caller, unique);
}

static void loop_work();
static void shutdown_others();
static void shutdown_loop();

void
listen_loop(void)
{
  NOTE_F;

  while (! cmpi_quitting)
  {
    // NOTE("loop");
    loop_work();
  }
  shutdown_others();
  shutdown_loop();
  DONE;
}

static void
loop_work()
{
  if (! MPIRPC_Check())
  {
    MPIRPC_Snooze();
    MPIRPC_Flush_returns();
  }
  else
  {
    MPIRPC_Snooze_reset();
  }
}

static void
shutdown_others()
{
  NOTE_F;
  DEBUG(dump_buckets());
  for (int i = 0; i < KDA_SPACE_SIZE; i++)
    for (struct list_item* item = k_bucket[i]->head; item;
        item = item->next)
    {
      KDA_Neighbor* neighbor = item->data;
      MPIRPC_Block(neighbor->node, "shutdown", NULL);
    }
  DONE;
}

static void
shutdown_loop()
{
  NOTE_F;
  time_t start = time(NULL);
  while (time(NULL) - start < 2)
    loop_work();
  DONE;
}

int KDA_Neighbor_table_size()
{
  int i;
  int result = 0;
  for (i = 0; i < KDA_SPACE_SIZE; i++)
    result += k_bucket[i]->size;
  return result;
}

char*
KDA_id_tostring(KDA_ID other_id)
{
  char* s_id = malloc(10*sizeof(char));
  sprintf(s_id, "%X", other_id);
  return s_id;
}

/**
  do not use this
  @deprecated
 */
void
dump_bucket_ids()
{
  char* result = bucket_ids_tostring();
  printf("%s", result);
  free(result);
}

/*
  do not use this
 */
char*
bucket_ids_tostring()
{
  // NOTE_F;
  char* result      =
    (char*) malloc(sizeof(char)*KDA_SPACE_SIZE*k*20);
  char* p = result;
  p += sprintf(p, "Neighbor table for: %X \n", id);
  for (int i = 0; i < KDA_SPACE_SIZE; i++)
  {
    bool bucket_named = false;
    for (struct list_item* item = k_bucket[i]->head; item;
        item = item->next)
    {
      if (! bucket_named)
      {
        bucket_named = true;
        p += sprintf(p, "bucket[%i]:\n", i);
      }
      p += sprintf(p, "\t");
      assert(item->data);
      KDA_Neighbor* neighbor = item->data;
      char* string = KDA_Neighbor_id_tostring(neighbor);
      p += sprintf(p, "%s (%i)\n", string, neighbor->node.rank);
    }
  }
  return result;
}

void
dump_buckets()
{
  // NOTE_F;
  char* result = buckets_tostring();
  printf("%s\n", result);
  free(result);
}

char*
buckets_tostring(void)
{
  // NOTE_F;
  char* result      =
    (char*) malloc(sizeof(char)*KDA_SPACE_SIZE*k*200);
  char* p = result;
  p += sprintf(p, "Neighbor table for: %X \n", id);
  for (int i = 0; i < KDA_SPACE_SIZE; i++)
  {
    bool bucket_named = false;
    for (struct list_item* item = k_bucket[i]->head; item;
         item = item->next)
    {
      if (! bucket_named)
      {
        bucket_named = true;
        p += sprintf(p, "bucket[%i]:\n", i);
      }
      p += sprintf(p, "\t");
      KDA_Neighbor* neighbor = item->data;
      assert(neighbor);
      char* string = KDA_Neighbor_tostring(neighbor);
      p += sprintf(p, "%s\n", string);
    }
  }
  return result;
}

/**
   Client call to shut down contacts
 */
void
KDA_Shutdown(struct list* contacts)
{
  NOTE_F;
  for (struct list_item* item = contacts->head; item;
       item = item->next)
  {
    KDA_Neighbor* neighbor = item->data;
    MPIRPC_Block(neighbor->node, "shutdown", NULL);
  }
}

char*
kda_op_tostring(KDA_Operation* op)
{
  char* result = malloc(1024);
  char* p = result;

  p += sprintf(p, "Op{");
  p += sprintf(p, "id=%i", op->id);

  return result;
}
