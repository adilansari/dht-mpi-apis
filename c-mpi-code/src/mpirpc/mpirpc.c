
#include <mpi_tools.h>

#include <unistd.h>

#include <itable.h>
#include <hashtable.h>
#include <list.h>

#include <mpirpc.h>

/**
   Counter to uniquify RPC calls.
*/
static int unique = 0;

/**
   For MPIRPC_Check() : Irecv()...
*/
// static MPI_Request check_request = MPI_REQUEST_NULL;

/**
   For MPIRPC_Check() : Irecv()...
*/
static int check_msg;

// Internal prototypes:
static void* MPIRPC_Remove(int index);
static void  MPIRPC_Handle(int msg, MPIRPC_Node caller);

/**
   Sendable call control code.
*/
static int mpirpc_msg_call;
/**
   Sendable return control code.
*/
static int mpirpc_msg_return;
/**
   Sendable shutdown control code.
*/
static int mpirpc_msg_disconnect;
/**
   Sendable OK control code.
*/
static int mpirpc_msg_ok;
/**
   Sendable failure control code.
*/
static int mpirpc_msg_fail;

/**
   Table of registered functions that may be called.
*/
static struct hashtable* portmap;

/**
   Table of outstanding RPCs.
*/
static struct itable* rpctable;

/**
   Table of known channels.
*/
static struct list* channels;

/**
   Return values to be garbage collected by MPIRPC_Check().
*/
static struct list* garbage_values;

/**
   Time in microseconds to snooze when MPIRPC_Check() finds nothing.
   snooze() & reset_snooze() are called by MPIRPC_Wait().
*/
static unsigned int snooze_amount_reset = 0;
static unsigned int snooze_amount = 0;

/**
   The maximum amount of time to snooze in microseconds.
   Default 500000us.
*/
static useconds_t snooze_max = 500000;

void
MPIRPC_Init()
{
  whoami();

  rpctable = itable_create(128);
  portmap  = hashtable_create(128);

  channels = list_create();

  garbage_values = list_create();

  mpirpc_msg_call       = MPIRPC_MSG_CALL;
  mpirpc_msg_return     = MPIRPC_MSG_RETURN;
  mpirpc_msg_disconnect = MPIRPC_MSG_DISCONNECT;
  mpirpc_msg_ok         = MPIRPC_MSG_OK;
  mpirpc_msg_fail       = MPIRPC_MSG_FAIL;
}

/**
   Register a new RPC for remote access.
*/
void
MPIRPC_Register(char* name, MPIRPC_Handler_pointer(f))
{
  hashtable_add(portmap, name, f);
}

/**
   Instruct MPIRPC to check this comm for incoming calls and returns.
*/
bool
MPIRPC_Comm_add(MPI_Comm comm)
{
  bool result;
  MPIRPC_Channel* channel =
    (MPIRPC_Channel*) malloc(sizeof(MPIRPC_Channel));
  channel->comm    = comm;
  channel->request = MPI_REQUEST_NULL;

  result =
    (bool) list_add_unique(channels, MPIRPC_Channel_cmp, channel);

  if (!result)
    free(channel);

  return result;
}

/**
   Locate the channel that this node is using.
*/
MPIRPC_Channel*
MPIRPC_Channel_for_node(MPIRPC_Node node)
{
  struct list_item* item;
  for (item = channels->head;
       item; item = item->next)
  {
    MPIRPC_Channel* channel = (MPIRPC_Channel*) item->data;
    if (channel->comm == node.comm)
      return channel;
  }
  return NULL;
}

struct list*
MPIRPC_Channels(void)
{
  return channels;
}

/**
   Close and remove comm from the channels list.
   Frees any nodes in channels containing comm.
   @return true iff the comm was found and disconnected.
*/
bool
MPIRPC_Comm_remove(MPI_Comm* comm)
{
  gossip_do(MASK_MPIRPC, NOTE_F);

  bool disconnected = false;

  struct list* matches =
    list_pop_where(channels, MPIRPC_Channel_comm_cmp, comm);
  assert(matches->size == 1);

  MPIRPC_Node* node = (MPIRPC_Node*) matches->head->data;
  MPI_Comm_disconnect(&node->comm);
  disconnected = true;

  list_destroy(matches);
  return true;
}

MPIRPC_Node*
MPIRPC_Node_create(MPI_Comm comm, int rank)
{
  MPIRPC_Node* node = malloc(sizeof(MPIRPC_Node));
  return MPIRPC_Node_make(comm, rank, node);
}

MPIRPC_Node*
MPIRPC_Node_make(MPI_Comm comm, int rank, MPIRPC_Node* node)
{
  node->comm = comm;
  node->rank = rank;
  return node;
}

MPIRPC*
MPIRPC_Create(MPIRPC_Node target, char* name, char* args,
              char* blob, int blob_length,
              void* extras, void (*proceed)(MPIRPC*))
{
  // gossip_do(MASK_MPIRPC, NOTE_F);
  if (!name)
  {
    printf("MPIRPC_Call(): name was NULL. \n");
    exit(1);
  }

  MPIRPC* rpc = malloc(sizeof(MPIRPC));
  strcpy(rpc->name, name);
  rpc->name_length = strlen(name);
  rpc->unique      = ++unique;
  rpc->target      = target;
  if (args)
  {
    strcpy(rpc->args, args);
    rpc->args_length = strlen(args);
  }
  else
    rpc->args_length = -1;
  rpc->blob        = blob;
  rpc->blob_length = blob_length;
  rpc->extras      = extras;
  rpc->proceed     = proceed;
  rpc->cancelled   = false;
  rpc->status      = MPIRPC_STATUS_PROTO;

  // gossip_do(MASK_MPIRPC, DONE);
  return rpc;
}

/**
   Issue an RPC.
   @param target The target MPI rank.
   @param name The RPC name in the remote portmap.
               Copied into MPIRPC.
   @param args The args to send.  Copied into MPIRPC.
   @param extras Extra state to pass to proceed.
   @param proceed The procedure to call when the RPC returns.
*/
MPIRPC*
MPIRPC_Call(MPIRPC_Node target, char* name, char* args,
            void* extras, void (*proceed)(MPIRPC* rpc))
{
  return MPIRPC_Call_blob(target, name, args,
                          NULL, 0, extras, proceed);
}

/**
   Issue an RPC.
   @param target The target MPI rank.
   @param name The RPC name in the remote portmap.
               Copied into MPIRPC.
   @param args The args to send.  Copied into MPIRPC.
   @param blob The blob to send, may be NULL.  Not copied.
   @param blob_length Length of the blob.
   @param extras Extra state to pass to proceed.
   @param proceed The procedure to call when the RPC returns.
*/
MPIRPC*
MPIRPC_Call_blob(MPIRPC_Node target, char* name, char* args,
                 void* blob, int blob_length,
                 void* extras, void (*proceed)(MPIRPC* rpc))
{
  assert(target.comm != MPI_COMM_NULL);

  MPIRPC* rpc = MPIRPC_Create(target, name, args,
                              blob, blob_length, extras, proceed);

  //  gossip_do(MASK_MPIRPC,
  //     char* commname = MPIRPC_Comm_get_name(target.comm));
  gossip_do(MASK_MPIRPC,
            SHOW_FISSI(target.rank, name,
                        args, rpc->unique));

  MPI_Isend(&mpirpc_msg_call, 1, MPI_INT, target.rank,
            MPIRPC_TAG_CONTROL, target.comm, &rpc->request[0]);

  MPI_Isend(&rpc->name_length, 1, MPI_INT, target.rank,
            MPIRPC_TAG_LENGTH, target.comm, &rpc->request[1]);

  MPI_Isend(name, rpc->name_length+1, MPI_CHAR, target.rank,
            MPIRPC_TAG_NAME, target.comm, &rpc->request[2]);

  MPI_Isend(&rpc->unique, 1, MPI_INT, target.rank,
            MPIRPC_TAG_UNIQUE, target.comm, &rpc->request[3]);

  MPI_Isend(&rpc->args_length, 1, MPI_INT, target.rank,
            MPIRPC_TAG_LENGTH, target.comm, &rpc->request[4]);

  if (rpc->args_length > 0)
    MPI_Isend(args, rpc->args_length+1, MPI_CHAR, target.rank,
              MPIRPC_TAG_ARGS, target.comm, &rpc->request[5]);

  MPI_Isend(&rpc->blob_length, 1, MPI_INT, target.rank,
            MPIRPC_TAG_LENGTH, target.comm, &rpc->request[6]);

  if (rpc->blob_length > 0)
    MPI_Isend(blob, blob_length, MPI_CHAR, target.rank,
              MPIRPC_TAG_ARGS, target.comm, &rpc->request[7]);

  rpc->status = MPIRPC_STATUS_CALLED;
  bool result = itable_add(rpctable, rpc->unique, rpc);
  assert(result);

  return rpc;
}

/**
   Calls MPIRPC_Free() before return.
   @param target The target MPI rank.
   @param name The RPC name in the remote portmap.
               Copied into MPIRPC.
   @param args The args to send.  Copied into MPIRPC.
*/
void*
MPIRPC_Block(MPIRPC_Node target, char* name, char* args)
{
  gossip_do(MASK_MPIRPC, NOTE_FS(name));
  return MPIRPC_Block_blob(target, name, args, NULL, 0);
}

void*
MPIRPC_Block_blob(MPIRPC_Node target, char* name, char* args,
                  void* blob, int blob_length)
{
  char* result;
  gossip_do(MASK_MPIRPC, NOTE_FS(name));
  MPIRPC* rpc = MPIRPC_Call_blob(target, name, args,
                                 blob, blob_length, NULL, NULL);
  MPIRPC_Wait(rpc);
  gossip_do(MASK_MPIRPC, NOTE_S("Unblocked: ", name));
  result = rpc->result;
  MPIRPC_Free(rpc);
  return result;
}

/**
   Receives RPC and issues it to local procedure handler.
   name is on the stack.
   Handler should free blob.
*/
void
MPIRPC_Request(MPIRPC_Node caller)
{
  MPI_Status status;

  char  name[MPIRPC_MAX_NAME];
  int   unique;
  int   name_length;
  char  args_space[MPIRPC_MAX_ARGS];
  int   args_length;
  char* args;
  int   blob_length;
  char* blob = NULL;

  // DEBUG(MPIRPC_Dump_channels());

  MPI_Recv(&name_length, 1, MPI_INT, caller.rank,
           MPIRPC_TAG_LENGTH, caller.comm, &status);
  MPI_Recv(name, name_length+1, MPI_CHAR, caller.rank,
           MPIRPC_TAG_NAME, caller.comm, &status);
  MPI_Recv(&unique, 1, MPI_INT, caller.rank,
           MPIRPC_TAG_UNIQUE, caller.comm, &status);
  MPI_Recv(&args_length, 1, MPI_INT, caller.rank,
           MPIRPC_TAG_LENGTH, caller.comm, &status);

  gossip_do(MASK_MPIRPC, SHOW_FSI(name,caller.rank));

  if (args_length > 0)
  {
    args = args_space;

    MPI_Recv(args, args_length+1, MPI_CHAR, caller.rank,
             MPIRPC_TAG_ARGS, caller.comm, &status);
  }
  else if (args_length == 0)
  {
    args = args_space;
    args[0] = '\0';
  }
  else
    args = NULL;

  MPI_Recv(&blob_length, 1, MPI_INT, caller.rank,
           MPIRPC_TAG_LENGTH, caller.comm, &status);

  if (blob_length > 0)
  {
    blob = malloc((blob_length)*sizeof(char));

    MPI_Recv(blob, blob_length, MPI_CHAR, caller.rank,
             MPIRPC_TAG_ARGS, caller.comm, &status);
  }

  char* (*handler)(MPIRPC_Node,int,char*,char*,int) =
    hashtable_search(portmap, name);

  if (handler == NULL)
    printf("No such method: %s \n", name);
  else
    handler(caller, unique, args, blob, blob_length);
}

/**
   Convenience for MPIRPC_Return(NULL).
   Return NULL to caller.
*/
void
MPIRPC_Null(MPIRPC_Node caller, int unique)
{
  MPIRPC_Return(caller, unique, NULL, -1);
}

MPIRPC_Value* MPIRPC_Value_create(int unique, char* result, int length);

/**
   Return result to caller for unique RPC.
   result is asynchronously freed upon completion.
   @param length Ignored if result == NULL.
*/
void
MPIRPC_Return(MPIRPC_Node caller, int unique,
              void* result, int length)
{
  gossip_do(MASK_MPIRPC, SHOW_FIII(caller.rank, unique, length));

  if (result == NULL)
    length = -1;

  MPIRPC_Value* value = MPIRPC_Value_create(unique, result, length);

  // SHOW_P(&value->request[0]);
  // NOTE_P("request 0: ", &value->request[0]);
  MPI_Isend(&mpirpc_msg_return, 1, MPI_INT, caller.rank,
            MPIRPC_TAG_CONTROL, caller.comm, &value->request[0]);
  MPI_Isend(&value->unique, 1, MPI_INT, caller.rank,
            MPIRPC_TAG_UNIQUE, caller.comm, &value->request[1]);
  // NOTE_P("request 2: ", &value->request[2]);
  MPI_Isend(&value->length, 1, MPI_INT, caller.rank,
            MPIRPC_TAG_LENGTH, caller.comm, &value->request[2]);
  if (value->length > 0)
  {
    MPI_Isend(value->result, value->length, MPI_CHAR, caller.rank,
              MPIRPC_TAG_RESULT, caller.comm, &value->request[3]);
  }

  list_push(garbage_values, value);
  MPIRPC_Garbage_collect();
}

/**
   The system should call this when we receive MPIRPC_MSG_RETURN.
   @return The MPIRPC that returned.
*/
MPIRPC*
MPIRPC_Retrieve(MPIRPC_Node sender)
{
  int i;

  MPI_Status status;
  int unique;

  MPI_Recv(&unique, 1, MPI_INT, sender.rank,
           MPIRPC_TAG_UNIQUE, sender.comm, &status);

  gossip_do(MASK_MPIRPC, SHOW_FII(sender.rank, unique));

  MPIRPC* rpc = (MPIRPC*) MPIRPC_Remove(unique);

  assert(rpc != NULL);

  MPI_Recv(&rpc->result_length, 1, MPI_INT, sender.rank,
           MPIRPC_TAG_LENGTH, sender.comm, &status);

  if (rpc->result_length > 0)
  {
    rpc->result = malloc((rpc->result_length)*sizeof(char));
    MPI_Recv(rpc->result, rpc->result_length, MPI_CHAR, sender.rank,
             MPIRPC_TAG_RESULT, sender.comm, &status);
  }
  else if (rpc->result_length == 0)
    rpc->result = NULL;
  else
    rpc->result = NULL;

  MPI_Request* r = rpc->request;
  for (i = 0; i < 5; i++)
    MPI_Request_free(&r[i]);
  if (rpc->args_length > 0)
    MPI_Request_free(&r[5]);
  MPI_Request_free(&r[6]);
  if (rpc->blob_length > 0)
    MPI_Request_free(&r[7]);

  rpc->status = MPIRPC_STATUS_RETURNED;
  if (rpc->proceed != MPIRPC_PROCEED_NULL &&
      rpc->proceed != NULL)
    rpc->proceed(rpc);
  return rpc;
}

/**
   Check for and handle any new RPC calls or returns.
   @return true iff some message was received.
*/
bool
MPIRPC_Check()
{
  bool       event = false;
  int        recvd = false;
  MPI_Status status;

  for (struct list_item* item = channels->head;
       item; item = item->next)
  {
    MPIRPC_Channel* channel = (MPIRPC_Channel*) item->data;
    // char* name = MPIRPC_Comm_get_name(channel->comm);

    if (channel->request == MPI_REQUEST_NULL)
    {
      // gossip_debug(MASK_MPIRPC, "Posting Irecv...\n");
      MPI_Irecv(&check_msg, 1, MPI_INT, MPI_ANY_SOURCE,
                MPIRPC_TAG_CONTROL, channel->comm,
                &channel->request);
    }
    MPI_Test(&channel->request, &recvd, &status);

    if (recvd)
    {
      event = true;
      MPIRPC_Node node;
      MPIRPC_Node_make(channel->comm, status.MPI_SOURCE, &node);
      MPIRPC_Handle(check_msg, node);
    }
  }

  return event;
}

/**
   Check for and handle a new RPC call or return.
*/
void
MPIRPC_Recv(MPI_Comm comm)
{
  MPI_Status status;
  int        check_msg;

  MPI_Recv(&check_msg, 1, MPI_INT, MPI_ANY_SOURCE,
           MPIRPC_TAG_CONTROL, comm, &status);

  MPIRPC_Node node;
  MPIRPC_Node_make(comm, status.MPI_SOURCE, &node);

  MPIRPC_Handle(check_msg, node);
}

MPIRPC_Value*
MPIRPC_Value_create(int unique, char* result, int length)
{
  MPIRPC_Value* value = malloc(sizeof(MPIRPC_Value));

  value->unique = unique;
  value->length = length;
  value->result = result;

  return value;
}

/**
   Flush and garbage collect all outstanding return values.
*/
void
MPIRPC_Flush_returns()
{
  // gossip_do(MASK_MPIRPC, NOTE_F);
  MPI_Status status;
  for (struct list_item* item = garbage_values->head; item;
      item = item->next)
  {
    MPIRPC_Value* value = (MPIRPC_Value*) item->data;
    for (int i = 0; i < 3; i++)
    {
      MPI_Wait(&value->request[i], &status);
    }

    if (value->length >= 0)
      MPI_Wait(&value->request[3], &status);
  }
}

/**
   Uses MPIRPC_Check() internally.
   Since this method returns rpc->result, you cannot used
   MPIRPC_Free() or MPIRPC_Destroy() as proceed-functions.
   @return rpc's result when obtained.
*/
void*
MPIRPC_Wait(MPIRPC* rpc)
{
  int unique = rpc->unique;
  if (rpc->status == MPIRPC_STATUS_RETURNED)
    return rpc->result;
  do
  {
    while (! MPIRPC_Check())
      MPIRPC_Snooze();
    MPIRPC_Snooze_reset();
    if (! itable_search(rpctable, unique))
      return rpc->result;
    else
      NOTE("Wait(): handled, still waiting...");
  } while (true);
  return NULL;
}

/**
   Indicate that this RPC will not be waited for or
   checked by the user so the system must deallocate it.
   It does not attempt to cancel any underlying MPI
   communication.
*/
void
MPIRPC_Cancel(MPIRPC* rpc)
{
  rpc->cancelled = true;
}

/**
   Lookup an outstanding function.
*/
/*
MPIRPC*
MPIRPC_Lookup(int index)
{
  return itable_search(rpctable, index);
}
*/

/**
   Called by MPIRPC_Retrieve().
*/
void*
MPIRPC_Remove(int index)
{
  return itable_remove(rpctable, index);
}

/**
   Inform remote node that channel will be closed and close channel.
*/
void
MPIRPC_Close(MPIRPC_Channel* channel)
{
  NOTE_F;

  int response;

  MPI_Send(&mpirpc_msg_disconnect, 1, MPI_INT,
           0, MPIRPC_TAG_CONTROL, channel->comm);
  MPI_Status status;
  MPI_Recv(&response, 1, MPI_INT,
           0, MPIRPC_TAG_CONTROL, channel->comm, &status);

  MPIRPC_Comm_remove(&channel->comm);

  assert(response == MPIRPC_MSG_OK);
}

void
MPIRPC_Garbage_collect()
{
  while (MPIRPC_Garbage_collect_value());
}

void MPIRPC_Value_free(MPIRPC_Value* value);

/**
   @return true iff we found and freed a sent value.
*/
bool
MPIRPC_Garbage_collect_value()
{
  MPIRPC_Value* value;
  MPI_Status    status;
  int           sent;

  value = list_head(garbage_values);
  if (!value)
    return false;

  for (int i = 0; i < 3; i++)
  {
    // NOTE_P("test: ", &value->request[i]);
    MPI_Test(&value->request[i], &sent, &status);
    if (!sent)
      return false;
  }
  if (value->length > 0)
  {
    MPI_Test(&value->request[3], &sent, &status);
    if (!sent)
      return false;
  }

  MPIRPC_Value_free(list_poll(garbage_values));
  return true;
}

void
MPIRPC_Value_free(MPIRPC_Value* value)
{
  if (value->result)
  {
    // NOTE_S("Free result: ", value->result);
    // SHOW_P(value->result);
    free(value->result);
  }

  // NOTE("Free value:");
  free(value);
}

void
MPIRPC_Disconnect(MPIRPC_Node caller)
{
  NOTE_F;

  MPI_Send(&mpirpc_msg_ok, 1, MPI_INT,
           0, MPIRPC_TAG_CONTROL, caller.comm);

  MPIRPC_Comm_remove(&caller.comm);
}

void
MPIRPC_Handle(int msg, MPIRPC_Node caller)
{
  // NOTE_F;
  switch (msg)
  {
    case MPIRPC_MSG_CALL:
      MPIRPC_Request(caller);
      break;
    case MPIRPC_MSG_RETURN:
      MPIRPC_Retrieve(caller);
      break;
    case MPIRPC_MSG_DISCONNECT:
      MPIRPC_Disconnect(caller);
      break;
    default:
      printf("bad msg from: %i\n", caller.rank);
      assert(false);
  }
}

/**
   Frees: the RPC (including the args).
   Does not free: the result, extras, and the blob.
   Must be called by the user (usually in the proceed function).
*/
void
MPIRPC_Free(MPIRPC* rpc)
{
  gossip_do(MASK_MPIRPC, SHOW_FSI(rpc->name, rpc->unique));
  free(rpc);
}

/**
   Frees: the RPC (including the args),
   the result, extras, and the blob.
   Must be called by the user (usually in the proceed function).
*/
void
MPIRPC_Destroy(MPIRPC* rpc)
{
  gossip_do(MASK_MPIRPC, SHOW_FSI(rpc->name, rpc->unique));

  if (rpc->result)
    free(rpc->result);
  if (rpc->extras)
    free(rpc->extras);
  if (rpc->result)
    free(rpc->blob);
  free(rpc);
}

void
MPIRPC_Snooze()
{
  if (snooze_amount > snooze_max)
    usleep(snooze_amount);
  else
    usleep(snooze_amount *= 2);
}

void
MPIRPC_Snooze_reset()
{
  snooze_amount = snooze_amount_reset;
}

void
MPIRPC_Snooze_set(int s)
{
  snooze_max = s;
}

/**
   Convenience method for MPI_Comm_get_name().
   Places the name in a static location.
*/
char*
MPIRPC_Comm_get_name(MPI_Comm comm)
{
  static char name[100];
  int  length;
  if (comm == MPI_COMM_NULL)
    sprintf(name, "MPI_COMM_NULL");
  else
    MPI_Comm_get_name(comm, name, &length);
  return name;
}

/**
   Compare channels based only on Comm.
   @param c1 MPIRPC_Channel*.
   @param c2 MPIRPC_Channel*.
*/
int
MPIRPC_Channel_cmp(void* o1, void* o2)
{
  MPIRPC_Channel* c1 = (MPIRPC_Channel*) o1;
  MPIRPC_Channel* c2 = (MPIRPC_Channel*) o2;
  if (c1->comm == c2->comm)
    return 0;
  else if (c1->comm < c2->comm)
    return -1;
  return 1;
}

/**
   Compares based on comm, then rank.
   Good for list_remove_where().
   @param node1 MPIRPC_Node*.
   @param node2 MPIRPC_Node*.
   Good for list_remove_where().
*/
int
MPIRPC_Node_cmp(void* n1, void* n2)
{
  MPIRPC_Node* node1 = (MPIRPC_Node*) n1;
  MPIRPC_Node* node2 = (MPIRPC_Node*) n2;
  if  (node1->comm == node2->comm &&
       node1->rank == node2->rank)
    return 0;
  else if (node1->comm > node2->comm)
    return 1;
  else if (node1->comm < node2->comm)
    return -1;
  else
    if (node1->rank > node2->rank)
      return 1;
    else
      return -1;
}

/**
   Compare channels based only on Comm.
   @param c1 MPIRPC_Channel*.
   @param c2 MPIRPC_Channel*.
 */
int
MPIRPC_Channel_comm_cmp(void* o1, void* o2)
{
  MPIRPC_Channel* c1   = (MPIRPC_Channel*) o1;
  MPI_Comm*       comm = (MPI_Comm*) o2;
  MPIRPC_Channel  channel;
  channel.comm = *comm;
  MPIRPC_Channel* c2 = &channel;

  return MPIRPC_Channel_cmp(c1, c2);
}

void
MPIRPC_Dump(MPIRPC* rpc)
{
  printf("[%i]:%i->[%i] %s(%s) \n",
         mpi_rank, rpc->unique, rpc->target.rank,
         rpc->name, rpc->args);
}

/**
   Show all registered functions.
*/
void MPIRPC_Dump_portmap(void)
{
  hashtable_dumpkeys(portmap);
}

/**
   Show all registered channels.
*/
void
MPIRPC_Dump_channels()
{
  struct list_item* item;
  printf("channels: \n");
  for (item = channels->head;
       item; item = item->next)
  {
    MPIRPC_Node* node = (MPIRPC_Node*) item->data;
    printf("\t %s \t\t %X \n", MPIRPC_Comm_get_name(node->comm), node->comm);
  }
}

void
MPIRPC_Finalize()
{
  gossip_do(MASK_MPIRPC, NOTE_F);

  list_destroy(channels);

  while (garbage_values->size > 0)
    MPIRPC_Garbage_collect();
  list_free(garbage_values);
  hashtable_free(portmap);
  itable_free(rpctable);
}

