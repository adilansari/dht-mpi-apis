
#ifndef KADEMLIA_TYPES_2_H
#define KADEMLIA_TYPES_2_H

#include <cmpi.h>

typedef CMPI_ID KDA_ID;

typedef struct neighbor
{
  MPIRPC_Node node;
  KDA_ID      id;
  time_t      lastseen;
} KDA_Neighbor;

/**
   Contains neighbors.  A bucket is all neighbors of a given
   distance 2^i - 2^i+1
*/
typedef struct list* K_BUCKET;

typedef struct kda_query     KDA_Query;
typedef struct kda_operation KDA_Operation;

/**
   Manages an out-of-DHT call.
*/
struct kda_query
{
  MPIRPC_Node caller;
  int         unique;
  char*       result;
  void        (*service)(KDA_Operation* op);
};

/**
   Performs an operation.
   1) Finds k nodes nearest to object_id using
      KDA_Find_node -> rpc_find_node -> proceed_find.
   2) When proceed_find converges, it
      calls map.
   3) map either maps the function name out (if name given),
      proceeding with the proceed function, or
      returns the query.
*/
struct kda_operation
{
  KDA_ID object_id;
  int    id;
  // An MPIRPC function symbolic name to call on found nodes:
  char*  name;
  char*  args;
  // Maps distance to neighbors:
  struct ilist* k_closest;
  // neighbors that have been searched but not heard from:
  struct list* outstanding;
  // neighbors that have been searched for this operation:
  struct list* contacted;
  // neighbors that have returned the RPC for this operation:
  struct list* returned;
  // If the latest rpc_find_node calls improved our search.
  bool   improved;
  int    status;
  // How to proceed after map(name):
  void   (*proceed)(MPIRPC* op);
  // Incoming query to respond to that triggered this operation:
  KDA_Query* query;
};

typedef struct
{
  void (*method)(MPIRPC_Node,void*);
  KDA_Neighbor* neighbor;
  void* args;
} KDA_RPC_Extras;

#endif
