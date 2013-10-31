
#ifndef KDA_2_H
#define KDA_2_H

#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <mpi.h>

#include <hashtable.h>
#include <itable.h>
#include <list.h>
#include <ilist.h>
#include <inlist.h>
#include <cmpi.h>
#include <mpi_tools.h>

#include "kda_types-2.h"

#include "connection.h"
#include "kda_neighbor-2.h"

#define KDA_SPACE_SIZE (sizeof(KDA_ID)*8) // 160 in paper
#define KDA_HASH_SPACE UINT_MAX           //

/**
   Obtain the Kademlia distance between two ids.
*/
#define XOR(id1, id2) (((unsigned int) id1) ^ ((unsigned int) id2))

extern KDA_ID   id;
extern K_BUCKET k_bucket[];
extern struct list* hubs;
extern int kda_nodes;

enum
{
  KDA_STATUS_SEARCHING,
  KDA_STATUS_CALLING,
  KDA_STATUS_COMPLETE
};

/**
   KDA_ID_NULL=0 is an invalid KDA_ID used for semantic purposes.
   make_id must not create this id.
*/
#define KDA_ID_NULL 0

/**
   KDA_ID_CLIENT=1 is an invalid KDA_ID used to identify client comms.
*/
#define KDA_ID_CLIENT 1

void KDA_Init(int alpha_in, int k_in);
void KDA_Init_client(int alpha_in, int k_in);

void KDA_Init_conn(void);
void KDA_Init_conn_client(void);

//// General API...
/**
   Return once op->status is KDA_STATUS_COMPLETE
*/
void KDA_Wait(KDA_Operation* op);

//// In-DHT API...
void  KDA_Ping(KDA_ID other_id);
int   KDA_Rank(KDA_ID other_id);
struct ilist* KDA_Closest(KDA_ID object_id);
struct ilist* KDA_Find_key(char* key);
struct ilist* KDA_Find(KDA_ID object_id);
MPIRPC_Node*  KDA_Node(KDA_ID other_id);

//// Helpers...

void KDA_Join(MPIRPC_Node node, KDA_Neighbor* neighbor);

void KDA_Setup_node_port(void);
// void KDA_Serve(void);
// void KDA_Serve_id(KDA_ID other_id, MPIRPC_Node node);
// MPIRPC_Node KDA_Serve_accept(void);
// void KDA_Serve_client(MPIRPC_Node node);
//void KDA_Serve_handshake(MPIRPC_Node node,
//                         KDA_ID* other_id, int* msg);
//void KDA_Serve_neighbor(KDA_ID other_id, MPIRPC_Node node);
// void KDA_Setup_connector_port(void);
// void KDA_Connect_port(char* port);
void KDA_Attach(struct list* contacts);
/**
   Attach to given neighbor if not already attached
 */
bool KDA_Attach_to(KDA_Neighbor* neighbor, struct list* contacts);
// MPIRPC_Node* KDA_Attach_port(char* port);
// MPIRPC_Node* KDA_Register_hub(MPI_Comm comm);

void KDA_Comm_set_name(KDA_Neighbor* neighbor);

KDA_Operation* KDA_Operate(KDA_ID object_id, char* name, void* args,
                           void (*proceed)(MPIRPC* rpc),
                           KDA_Query* query);
bool KDA_Find_node(KDA_Operation* op);
bool KDA_Find_node_again(KDA_Operation* op);

// void KDA_Send_neighbor(MPI_Comm comm);
void KDA_Serve_shutdown(void);
void KDA_Detach(MPIRPC_Node node);
void KDA_Detach_hubs();
void KDA_Shutdown(struct list* contacts);
void KDA_Shutdown_port(char* hub_port);

/**
   Return the KDA_Neighbor for this MPIRPC_Node.
   Could fail if other_id is not in local neighbor table.
*/
KDA_Neighbor* KDA_Neighbor_node(MPIRPC_Node node);

/**
   Could fail if other_id is not in local neighbor table.
 */
KDA_Neighbor* KDA_Neighbor_ID(KDA_ID other_id);

/**
   Attempt to contact the node other_id using a node as a
   reference.
*/
void KDA_Harpoon(MPIRPC_Node reference, KDA_ID other_id);

/**
   Connect two neighbors in the DHT.
   @param node1 The neighbor to issue the accept().
   @param node2 The neighbor to issue the connect().
*/
void KDA_Link(MPIRPC_Node node1, MPIRPC_Node node2);

/**
   @return A description of the form "id".
*/
char* KDA_Description(void);
char* KDA_id_tostring(KDA_ID other_id);

KDA_Neighbor* KDA_Random_neighbor(void);

//// Remote API:
MPIRPC* KDA_Translate(MPIRPC_Node node, KDA_ID id);
MPIRPC* KDA_Translate_k(MPIRPC_Node node, KDA_ID id);
MPIRPC* KDA_Lookup(MPIRPC_Node node, char* key);
MPIRPC* KDA_Lookup_k(MPIRPC_Node node, char* key);

MPIRPC* KDA_Store(KDA_Neighbor* neighbor, char* key, char* value,
		  int length);
MPIRPC* KDA_Retrieve(KDA_Neighbor* neighbor, char* key);
MPIRPC* KDA_Update(KDA_Neighbor* neighbor, char* key, char* value,
		   int length, int offset);


void listen_loop(void);

bool neighbor_add(KDA_Neighbor* neighbor);

#endif
