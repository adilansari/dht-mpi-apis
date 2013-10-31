
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <accessor.h>

#include "test-cmd.h"

int sleep_time;

void test_cmd_setup(int argc, char* argv[])
{
  char* dir = NULL;

  int c;
  while ((c = getopt(argc, argv, "d:s:")) != -1)
  {
    int n;
    switch (c)
    {
      case 'd':
      {
        // Set driver directory:
        dir = malloc(1024);
        n = sscanf(optarg, "%s", dir);
        if (n == 0)
        {
          printf("test-cmd -d got no argument!\n");
          exit(1);
        }
        break;
      }
      case 's':
      {
        // Set sleep interval for test-cmd-sleep
        n = sscanf(optarg, "%i", &sleep_time);
        if (n == 0)
        {
          printf("test-cmd -s got no argument!\n");
          exit(1);
        }
        break;
      }
      default:
      {
        printf("test-cmd got bad argument!\n");
        exit(1);
      }
    }
  }

  // printf("cmd -d %s\n", dir);

  accessor_fifo_setup(dir);
}
