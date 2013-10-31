
#ifndef DENSE_1_H
#define DENSE_1_H

#include <cmpi.h>
#include <inlist.h>
#include <string.h>
#include <unistd.h>

#ifdef USE_STRINGS_H

#endif

typedef int DENSE_ID;

extern int  dense_nodes;
extern bool dense_quitting;

void    DENSE_Init(int k_in, MPI_Comm comm);
void    DENSE_Init_client(void);
MPIRPC* DENSE_Lookup(MPIRPC_Node node, char* key);
MPIRPC* DENSE_Lookup_k(MPIRPC_Node node, char* key);
MPIRPC* DENSE_Translate(MPIRPC_Node node, DENSE_ID id);
MPIRPC* DENSE_Translate_k(MPIRPC_Node node, DENSE_ID id);
MPIRPC* DENSE_Store(MPIRPC_Node node, char* key, char* value,
                    int length);
MPIRPC* DENSE_Update(MPIRPC_Node node, char* key, char* value,
                     int length, int offset);
MPIRPC* DENSE_Retrieve(MPIRPC_Node node, char* key);
void    DENSE_Shutdown(MPIRPC_Node node);
void    DENSE_Debug_cache(MPIRPC_Node node);

void rpc_bootping(void);
void listen_loop(void);

#endif
