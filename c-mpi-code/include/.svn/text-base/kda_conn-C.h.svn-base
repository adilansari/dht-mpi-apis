
/**
    Connection techniques for KDA-2B DHT.
    Uses HUBs to manage initial bootstrap connections.
    Uses three-way link/accept/connect/join to add neighbors. 
*/

extern char port[]; 

void KDA_Init_conn(void); 

void KDA_Connect_port(char* port); 

//// Hub methods: 
void KDA_Hub_init(void); 
void KDA_Hub_loop(void); 
MPIRPC_Node* KDA_Serve_accept(void);
void KDA_Serve_handshake(MPIRPC_Node* node, KDA_ID* other_id,
                         int* msg); 
void KDA_Serve_id(KDA_ID other_id, MPIRPC_Node* node); 
void KDA_Serve_client(MPIRPC_Node* node); 
void KDA_Serve_neighbor(KDA_ID other_id, MPIRPC_Node* node); 

void KDA_Node_init(void);

void KDA_Init_conn_client(void); 

void KDA_Setup_node_port(void);

/**
   After establishing connection via MPI-2,
   connector calls acceptor to update neighbor tables. 
*/
void KDA_Join(MPI_Comm comm); 


MPIRPC_Node* KDA_Attach(void); 

MPIRPC_Node* KDA_Attach_port(char* hub_port); 

/**
   Register this comm as a hub. 
*/ 
MPIRPC_Node* KDA_Register_hub(MPI_Comm comm); 

/**
   Link the caller node to the requested node. 
*/
void handle_link(MPIRPC_Node* caller, int unique, char* args); 

/**
   Connect two neighbors in the DHT, if neither is NULL.  
   @param n1 The neighbor to issue the accept(). 
   @param n2 The neighbor to issue the connect().
*/
void KDA_Link(MPIRPC_Node* node1, MPIRPC_Node* node2); 

void handle_accept(MPIRPC_Node* caller, int unique, char* args); 

void handle_connect(MPIRPC_Node* caller, int unique, char* args); 

KDA_ID rpc_join(MPIRPC_Node* node);
void   rpc_link_neighbor(MPIRPC_Node* node, KDA_ID other_id); 
