
/**
    Connection techniques for KDA-2B DHT.
    Uses HUBs to manage initial bootstrap connections.
    Uses three-way link/accept/connect/join to add neighbors.
 */

#include "kda-2.h"
#include "conn-B.h"

int unique_client;
char port[MPI_MAX_PORT_NAME];

//// Internal prototypes
int thirdhash(char* s);

void
KDA_Init_conn()
{
  NOTE_F;

  KDA_Setup_node_port();

  char kda_port[MPI_MAX_PORT_NAME];
  int err = MPI_Lookup_name("kademlia", MPI_INFO_NULL, kda_port);
  if (err == MPI_SUCCESS)
  {
    // This node will be a DHT node:
    KDA_Node_init();
    KDA_Connect_port(kda_port);
    KDA_Ping(id);
    listen_loop();
  }
  else
  {
    // This node is a HUB:
    KDA_Hub_init();
    KDA_Hub_loop();
  }
}

void
KDA_Node_init()
{
  MPIRPC_Register("accept",  handle_accept);
  MPIRPC_Register("connect", handle_connect);
  MPIRPC_Register("join",       handle_join);
}

void
KDA_Client_init(void)
{
  MPIRPC_Register("connect", handle_connect);
  MPIRPC_Register("join",       handle_join);
}

void
KDA_Connect_port(char* port)
{
  NOTE_F;
  MPI_Comm newcomm;
  MPI_Comm_connect(port, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &newcomm);
  // MPI_Comm_set_errhandler(newcomm, MPI_ERRORS_RETURN);
  MPIRPC_Comm_add(newcomm);
  MPI_Comm_set_name(newcomm, "HUB");
  MPI_Send(&id,            1, MPI_INT, 0,
           CMPI_TAG_ID, newcomm);
  MPI_Send(&cmpi_msg_join, 1, MPI_INT, 0,
           CMPI_TAG_CONTROL, newcomm);
}

void
KDA_Hub_init(void)
{
  NOTE_F;
  MPI_Open_port(MPI_INFO_NULL, port);
  MPI_Publish_name("kademlia", MPI_INFO_NULL, port);
  NOTE_S("Published kademlia on: %s", port);
}

void
KDA_Init_conn_client()
{
  NOTE_F;
  KDA_Setup_node_port();
}

void
KDA_Setup_node_port()
{
  NOTE_F;
  MPI_Open_port(MPI_INFO_NULL, port);
  char portnumber[10];
  memset(portnumber, '\0', (size_t) 10);
  int p = thirdhash(port);
  strncpy(portnumber, port+p+1, 5);
  NOTE_S("Using port: ", portnumber);
}

void
KDA_Hub_loop()
{
  NOTE_F;

  int    msg;
  KDA_ID other_id;
  while (! cmpi_quitting)
  {
    MPIRPC_Node* node = KDA_Serve_accept();
    KDA_Serve_handshake(node, &other_id, &msg);

    switch (msg)
    {
      case CMPI_MSG_JOIN:
        KDA_Serve_id(other_id, node);
        break;
      case CMPI_MSG_SHUTDOWN:
        KDA_Serve_shutdown();
        break;
    }
  }
}

MPIRPC_Node*
KDA_Serve_accept()
{
  NOTE_F;
  MPI_Comm newcomm;
  MPI_Comm_accept(port, MPI_INFO_NULL, 0,
                  MPI_COMM_WORLD, &newcomm);
  // MPI_Comm_set_errhandler(newcomm, MPI_ERRORS_RETURN);
  char name[10];
  sprintf(name, "client_%i", unique_client++);
  MPI_Comm_set_name(newcomm, name);
  MPIRPC_Node* node = MPIRPC_Node_create(newcomm, 0);
  SHOW_S(name);
  SHOW_X(newcomm);

  return node;
}

/**
   TODO: Replace Recv()s with new reliable timeout Trecv()s.
*/
void
KDA_Serve_handshake(MPIRPC_Node* node, KDA_ID* other_id, int* msg)
{
  MPI_Status status;
  MPI_Comm   comm = node->comm;
  MPI_Recv(other_id, 1, MPI_INT, 0, CMPI_TAG_ID,      comm, &status);
  MPI_Recv(msg,      1, MPI_INT, 0, CMPI_TAG_CONTROL, comm, &status);
}

void
KDA_Serve_id(KDA_ID other_id, MPIRPC_Node* node)
{
  assert(other_id != KDA_ID_NULL);

  if (other_id == KDA_ID_CLIENT)
    KDA_Serve_client(node);
  else
    KDA_Serve_neighbor(other_id, node);
}

void
KDA_Serve_client(MPIRPC_Node* node)
{
  NOTE_F;
  MPIRPC_Comm_add(node->comm);
  KDA_Neighbor* neighbor = random_neighbor();
  if (neighbor)
    KDA_Link(neighbor->node, node);
  else
    NOTE("Could not get random neighbor.");
}

void
KDA_Serve_neighbor(KDA_ID other_id, MPIRPC_Node* node)
{
  NOTE_F;
  KDA_Neighbor* neighbor = KDA_Neighbor_create_id(other_id, node);
  MPIRPC_Comm_add(node->comm);
  KDA_Neighbor* rand_neighbor = random_neighbor();
  if (rand_neighbor)
    KDA_Link(rand_neighbor->node, node);
  else
    NOTE("Could not get random neighbor.");
  if (! neighbor_add(neighbor))
    free(neighbor);
  dump_buckets();
}

/**
   Attach
 */
MPIRPC_Node*
KDA_Attach()
{
  NOTE_F;

  KDA_Client_init();

  MPIRPC_Node* result = NULL;

  char kda_port[MPI_MAX_PORT_NAME];
  int err = MPI_Lookup_name("kademlia", MPI_INFO_NULL, kda_port);
  if (err == MPI_SUCCESS)
  {
    result = KDA_Attach_port(kda_port);
    while (! MPIRPC_Check());  // connect call
    while (! MPIRPC_Check());  // join call
  }

  return result;
}

MPIRPC_Node*
KDA_Attach_port(char* port)
{
  NOTE_F;

  MPI_Comm newcomm;
  MPI_Comm_connect(port, MPI_INFO_NULL, 0,
                   MPI_COMM_WORLD, &newcomm);
  NOTE("connected to HUB");
  NOTE_I("id: ", id);

  MPIRPC_Node* hub = KDA_Register_hub(newcomm);

  // MPI_Comm_set_errhandler(newcomm, MPI_ERRORS_RETURN);
  MPI_Send(&id,            1, MPI_INT, 0,
           CMPI_TAG_ID, newcomm);
  MPI_Send(&cmpi_msg_join, 1, MPI_INT, 0,
           CMPI_TAG_CONTROL, newcomm);

  return hub;
}

/**
   Register this comm as a hub.
*/
MPIRPC_Node*
KDA_Register_hub(MPI_Comm comm)
{
  MPI_Comm_set_name(comm, "HUB");
  MPIRPC_Node* hub = MPIRPC_Node_create(comm, 0);
  list_add(hubs, hub);
  MPIRPC_Comm_add(comm);
  return hub;
}

/**
   Link the caller node to the requested node.
*/
void
handle_link(MPIRPC_Node* caller, int unique, char* args)
{
  NOTE_F;
  KDA_ID rID;
  sscanf(args, "%X", &rID);
  NOTE_FX(rID);
  // dump_buckets();
  // KDA_Neighbor* neighbor = KDA_Neighbor_ID(rID);
  // KDA_Perform_link(caller, neighbor->node);
  MPIRPC_Return(caller, unique, NULL);
}

void
KDA_Harpoon(MPIRPC_Node* node, KDA_ID other_id)
{
  MPIRPC_Block(node, "link", xheap(other_id));
  NOTE("rpc_link_neighbor() done");
}

/**
   Connect two neighbors in the DHT, if neither is NULL.
   @param n1 The neighbor to issue the accept().
   @param n2 The neighbor to issue the connect().
*/
void
KDA_Link(MPIRPC_Node* node1, MPIRPC_Node* node2)
{
  NOTE_F;

  if (node1 == NULL || node2 == NULL)
    return;

  char* port   = MPIRPC_Block(node1, "accept", NULL);
  char* result = MPIRPC_Block(node2, "connect", port);

  dump_buckets();

  free(port);
  free(result);
}

void
handle_accept(MPIRPC_Node* caller, int unique, char* args)
{
  NOTE_F;
  MPIRPC_Return(caller, unique, port);
  MPIRPC_Flush_returns();
  MPI_Comm newcomm;
  MPI_Comm_accept(port, MPI_INFO_NULL, 0,
                  MPI_COMM_WORLD, &newcomm);
  NOTE("accepted newcomm");
  MPI_Comm_set_name(newcomm, "COMM_ACCEPTED");
  MPIRPC_Comm_add(newcomm);
  KDA_Join(newcomm);
}

void
handle_connect(MPIRPC_Node* caller, int unique, char* args)
{
  NOTE_FS(args);
  MPI_Comm newcomm;
  int err = MPI_Comm_connect(args, MPI_INFO_NULL, 0,
                             MPI_COMM_WORLD, &newcomm);

  MPI_Comm_set_name(newcomm, "COMM_CONNECTED");
  if (err == MPI_SUCCESS)
  {
    MPIRPC_Comm_add(newcomm);
    MPIRPC_Return(caller, unique, "success");
  }
  else
    MPIRPC_Return(caller, unique, "fail");
}

KDA_ID
rpc_join(MPIRPC_Node* node)
{
  KDA_ID other_id;
  char*  result = MPIRPC_Block(node, "join", id_tostring());
  sscanf(result, "%X", &other_id);
  free(result);

  NOTE("rpc_join returning");

  return other_id;
}

void
handle_join(MPIRPC_Node* caller, int unique, char* args)
{
  NOTE_FS(args);

  KDA_ID other_id;
  sscanf(args, "%X", &other_id);

  KDA_Neighbor* neighbor = KDA_Neighbor_create_id(other_id, caller);
  if (other_id != KDA_ID_CLIENT)
  {
    if (! neighbor_add(neighbor))
      free(neighbor);
  }
  else
  {
    client_add(neighbor);
  }
  KDA_Comm_set_name(neighbor);

  dump_buckets();

  MPIRPC_Return(caller, unique, xheap(id));
  MPIRPC_Flush_returns();
  // KDA_Ping(id);
}

void
KDA_Join(MPI_Comm comm)
{
  NOTE_F;

  MPIRPC_Node*  node     = MPIRPC_Node_create(comm, 0);
  KDA_ID        other_id = rpc_join(node);

  assert(other_id != KDA_ID_NULL);

  if (other_id != KDA_ID_CLIENT)
  {
    KDA_Neighbor* neighbor = KDA_Neighbor_create_id(other_id, node);

    // dump_buckets();

    if (! neighbor_add(neighbor))
    {
      NOTE("ALREADY HAVE NEIGHBOR? ");
      KDA_Neighbor* tmp = neighbor;
      neighbor = neighbor_lookup(neighbor);
      neighbor_set_node(neighbor, node);
      free(tmp);
    }
    KDA_Comm_set_name(neighbor);

    dump_buckets();
  }
  else
  {
    MPI_Comm_set_name(node->comm, "KDA_CLIENT");
  }
}

/**
   Finds index of the third hash character in the string.
   Useful for obtaining the port number from an MPI port name.
*/
int
thirdhash(char* s)
{
  char* p = s;
  int hashes = 0;
  while (p++)
  {
    if (*p == '#')
      hashes++;
    if (hashes == 3)
      return p-s;
  }
  printf("ERROR in thirdhash() \n");
  exit(1);
  return -1;
}


void
KDA_Shutdown()
{
  NOTE_F;
  char hub_port[MPI_MAX_PORT_NAME];
  int err = MPI_Lookup_name("kademlia", MPI_INFO_NULL, hub_port);
  if (err == MPI_SUCCESS)
  {
    KDA_Shutdown_port(hub_port);
  }
  else
  {
    note("shutdown error.");
  }
}

void
KDA_Shutdown_port(char* hub_port)
{
  MPI_Comm newcomm;
  MPI_Comm_connect(hub_port, MPI_INFO_NULL, 0,
                   MPI_COMM_WORLD, &newcomm);
  NOTE("connected");
  // MPI_Comm_set_errhandler(newcomm, MPI_ERRORS_RETURN);
  MPI_Comm_set_name(newcomm, "HUB_SHUTDOWN");
  MPI_Send(&id,                1, MPI_INT, 0,
           CMPI_TAG_ID, newcomm);
  MPI_Send(&cmpi_msg_shutdown, 1, MPI_INT, 0,
           CMPI_TAG_CONTROL, newcomm);
}

void
KDA_Serve_shutdown()
{
  NOTE_F;

  int i;
  struct list_item* item;
  NOTE_F;
  cmpi_quitting = true;
  for (i = 0; i < KDA_SPACE_SIZE; i++)
    for (item = k_bucket[i]->head;
         item; item = item->next)
    {
      KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
      MPIRPC_Block(neighbor->node, "shutdown", NULL);
      //KDA_Neighbor_dump(neighbor);
    }
}

void
KDA_Detach(MPIRPC_Node* node)
{
  MPIRPC_Channel* channel = MPIRPC_Channel_for_node(node);
  MPIRPC_Close(channel);
}

