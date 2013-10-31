
#ifndef MPIRPC_H
#define MPIRPC_H

#include <cmpi-config.h>

#include <mpi.h>

#include <stdbool.h>

#define MPIRPC_MAX_NAME 128
#define MPIRPC_MAX_ARGS 256
#define MPIRPC_PROCEED_NULL ((void (*)(MPIRPC*)) 1)

typedef struct
{
  MPI_Comm    comm;
  int         rank;
} MPIRPC_Node;

typedef struct
{
  MPI_Comm    comm;
  MPI_Request request;
} MPIRPC_Channel;

typedef struct mpirpc
{
  MPIRPC_Node target;
  int   status;
  char  name[MPIRPC_MAX_NAME];
  char  args[MPIRPC_MAX_ARGS];
  void* blob;
  int   blob_length;
  void* result;
  int   result_length;
  void* extras;
  void  (*proceed) (struct mpirpc* rpc);
  int   unique;
  bool  cancelled;

  // Internals:
  /**
     name_length is transferred via Isend().
  */
  int name_length;

  /**
     args_length is transferred via Isend().
  */
  int args_length;

  /**
     Array of Isend() requests to be freed on return.
  */
  MPI_Request request[8];
} MPIRPC;

/**
   Internal MPIRPC return value.  Used for garbage collection.
*/
typedef struct
{
  int   unique;
  int   length;
  void* result;
  MPI_Request request[4];
} MPIRPC_Value;

/**
   Defines MPIRPC handler "f"
   Parameters:
      the target node
      the uniqifier
      the args
      the blob
      the blob length
 */
#define MPIRPC_Handler_pointer(f) void \
        (*f)(MPIRPC_Node,int,char*,void*,int)

#define MPIRPC_Handler(f) void \
        f(MPIRPC_Node caller, int unique, char* args, void* blob, int blob_length)

//// API...

void MPIRPC_Init(void);

bool MPIRPC_Check(void);

void MPIRPC_Recv(MPI_Comm comm);

void MPIRPC_Register(char* name, MPIRPC_Handler_pointer(f));

bool MPIRPC_Comm_add(MPI_Comm comm);

void MPIRPC_Close(MPIRPC_Channel* channel);

MPIRPC* MPIRPC_Call(MPIRPC_Node target, char* name, char* args,
                    void* extras, void (*proceed)(MPIRPC*));

MPIRPC* MPIRPC_Call_blob(MPIRPC_Node target, char* name, char* args,
                         void* blob, int blob_length,
                         void* extras, void (*proceed)(MPIRPC*));

void* MPIRPC_Block(MPIRPC_Node target, char* name, char* args);

void* MPIRPC_Block_blob(MPIRPC_Node target, char* name, char* args,
                        void* blob, int blob_length);

void* MPIRPC_Wait(MPIRPC* rpc);

void MPIRPC_Null(MPIRPC_Node caller, int unique);

void MPIRPC_Return(MPIRPC_Node caller, int unique,
                   void* result, int rlength);

MPIRPC_Node* MPIRPC_Node_create(MPI_Comm comm, int rank);

MPIRPC_Node* MPIRPC_Node_make(MPI_Comm comm, int rank,
                              MPIRPC_Node* node);

void MPIRPC_Garbage_collect(void);
bool MPIRPC_Garbage_collect_value(void);

// struct list* MPIRPC_Comms(void);

void MPIRPC_Dump(MPIRPC* rpc);

void MPIRPC_Dump_portmap(void);

void MPIRPC_Dump_channels(void);

void MPIRPC_Flush_returns(void);

char* MPIRPC_Comm_get_name(MPI_Comm comm);

void MPIRPC_Free(MPIRPC* rpc);

void MPIRPC_Destroy(MPIRPC* rpc);

void MPIRPC_Snooze(void);

void MPIRPC_Snooze_reset(void);

void MPIRPC_Snooze_set(int s);

//// Data structure helpers...

int MPIRPC_Node_cmp(void* node1, void* node2);

int MPIRPC_Channel_cmp(void* c1, void* c2);

int MPIRPC_Channel_comm_cmp(void* c1, void* c2);

MPIRPC_Channel* MPIRPC_Channel_for_node(MPIRPC_Node node);

void MPIRPC_Finalize(void);

/**
   MPIRPC stati.
*/
enum
{
  MPIRPC_STATUS_PROTO,
  MPIRPC_STATUS_CALLED,
  MPIRPC_STATUS_RETURNED
};

/**
   Tags used by MPIRPC.
*/
enum
{
  MPIRPC_TAG_CONTROL,
  MPIRPC_TAG_UNIQUE,
  MPIRPC_TAG_NAME,
  MPIRPC_TAG_LENGTH,
  MPIRPC_TAG_ARGS,
  MPIRPC_TAG_RESULT
};

enum
{
  MPIRPC_MSG_CALL       = 12,
  MPIRPC_MSG_RETURN     = 13,
  MPIRPC_MSG_DISCONNECT = 14,
  MPIRPC_MSG_OK         = 15,
  MPIRPC_MSG_FAIL       = 16
};

#endif
