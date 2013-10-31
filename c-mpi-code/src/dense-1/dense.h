
#include <mpirpc-2.h> 

void DENSE_Init(int k_in, MPI_Comm comm); 
void handle_info(MPIRPC_Node* caller, int unique, char* args); 
void handle_ping(MPIRPC_Node* caller, int unique, char* args); 
void handle_query_id(MPIRPC_Node* caller, int unique, char* args); 
void handle_query_id_k(MPIRPC_Node* caller, int unique, char* args);
void handle_store(MPIRPC_Node* caller, int unique, char* args); 
void handle_retrieve(MPIRPC_Node* caller, int unique, char* args); 

