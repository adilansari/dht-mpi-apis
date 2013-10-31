
#include <node.h>
#include <cmpi-driver.h>

#include "test_helpers.h"

void
cmpi_client_code()
{
  wait_for_notification();
  notify_next();

  NOTE("PROCEEDING...");

  cmpi_driver* driver = driver_create();

  while (! driver->quitting)
  {
    driver_setup_fifo(driver);
    driver_process(driver);
  }

  NOTE("QUITTING");
}
