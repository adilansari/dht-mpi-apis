
#include <cmpi-config.h>

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmpi.h>
#include <io_tools.h>

/**
   Maximum command line for driver.
*/
#define CMPI_DRIVER_MAX_COMMAND (100+CMPI_KEY_LENGTH)

typedef struct
{
  FILE* source;
  FILE* sink;
  char command[CMPI_DRIVER_MAX_COMMAND];
  char data[CMPI_VALUE_LENGTH];
  bool quitting;
} cmpi_driver;

enum
{
  CMPI_DRIVER_FILE,
  CMPI_DRIVER_STDIN,
  CMPI_DRIVER_PIPE
};

//// API...

cmpi_driver* driver_create(void);

bool driver_setup_stdin(cmpi_driver* driver);
bool driver_setup_file(cmpi_driver* driver, char* filename);
bool driver_setup_fifo(cmpi_driver* driver);

void driver_process(cmpi_driver* driver);

//// Internal functions...

struct list* driver_tokens(cmpi_driver* driver);



