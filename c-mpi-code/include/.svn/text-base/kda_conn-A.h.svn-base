
#include "kda-2.h"

void KDA_Init_conn(void);

MPI_Comm KDA_Comm_create(int* other_rank);

/**
   Translate a KDA_ID to a rank in MPI_COMM_WORLD.
*/
MPIRPC_Handler(handle_get_rank);

/**
   Return the local KDA_ID.
*/
MPIRPC_Handler(handle_get_id);
