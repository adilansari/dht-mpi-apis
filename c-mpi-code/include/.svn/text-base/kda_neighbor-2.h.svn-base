
#ifndef KDA_NEIGHBOR_2_H
#define KDA_NEIGHBOR_2_H

#include <stdbool.h>

#include "kda_types-2.h"
#include "kda-2.h"

/**
   Scan the stream data for neighbors.
   @return The new neighbor.  n contains the number of chars consumed
   from data by sscanf().
*/
KDA_Neighbor* KDA_Neighbor_create_scan(char* data, int* n);
KDA_Neighbor* KDA_Neighbor_create_string(char* info);
KDA_Neighbor* KDA_Neighbor_create_id(KDA_ID id, MPIRPC_Node node);
void KDA_Neighbor_make_id(KDA_ID other_id, MPIRPC_Node node,
                          KDA_Neighbor* output);
KDA_Neighbor* KDA_Neighbor_clone(KDA_Neighbor* original);

//// cmp()s...
int KDA_Neighbor_node_cmp(void* object1, void* object2);

/**
   Compare ids.
   @return 1 iff n1 > n2, -1 iff n1 < n2, 0 iff n1 == n2
*/
int KDA_Neighbor_cmp(void* object1, void* object2);

/**
   Compare lastseen times for neighbors n1, n2.
   @return 1 iff n1 > n2, -1 iff n1 < n2, 0 iff n1 == n2
*/
int KDA_Neighbor_time_cmp(void* object1, void* object2);

//// Output methods:
/**
   Writes neighbor id to static output location.
*/
char* KDA_Neighbor_id_tostring(void* object);
char* KDA_Neighbor_name(void* object);
// int   KDA_Neighbor_name_sprint(char* buffer, void* object);
char* KDA_Neighbor_tostring(void* object);
int   KDA_Neighbor_sprint(char* buffer, void* object);
void  KDA_Neighbor_dump(void* object);

//// Input methods:
struct list*  KDA_Neighbor_node_list(char* data);
KDA_Neighbor* KDA_Neighbor_scan(char* data, int* n);

#endif
