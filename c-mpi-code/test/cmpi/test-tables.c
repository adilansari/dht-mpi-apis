
/**
   Assemble and debug tables.
*/

#include <unistd.h>

#include <cmpi.h>

#include "test_helpers.h"

void
cmpi_client_code()
{
  NOTE_F;

  gossip_set_debug_mask(1, MASK_MPIRPC|MASK_DHT);

  // wait_for_notification();
  // notify_next();

  sleep(2);

  cmpi_debug_tables();

  sleep(mpi_rank);

  if (mpi_rank == mpi_size-1)
    cmpi_shutdown();
}
