
/**
   Simply build DHT and shutdown.
   Be sure to pass -n <nodes> !
*/

#include <unistd.h>

#include "test_helpers.h"

void
cmpi_client_code()
{
  NOTE_F;

  gossip_set_debug_mask(1, MASK_MPIRPC|MASK_DHT);

  puts("sleeping...");

  sleep(4);
  // wait_for_notification();
  // puts("notifying...");
  // notify_next();

  puts("running...");

  cmpi_shutdown();
}
