
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <accessor.h>
#include <io_tools.h>

char* fifo_in = NULL;
char* fifo_out = NULL;

FILE* to_cmpi;
FILE* from_cmpi;

#define CHECK_ERROR                             \
  if (error != CMPI_SUCCESS)                    \
    return error;

void
accessor_fifo_setup(char* dir)
{
  if (!dir)
    dir = "/tmp";

  fifo_in = malloc(1024*sizeof(char));
  fifo_out = malloc(1024*sizeof(char));

  fifo_in[0] = '\0';
  fifo_out[0] = '\0';

  strcat(fifo_in, dir);
  strcat(fifo_in, "/cmpi.fifo.in");

  strcat(fifo_out, dir);
  strcat(fifo_out, "/cmpi.fifo.out");

  // printf("fifo_in:  %s\n", fifo_in);
  // printf("fifo_out: %s\n", fifo_out);
}

/**
   @return True iff successfully access to fifos is made.
*/
bool
accessor_fifo_init()
{
  assert(fifo_in);

  to_cmpi = fopen(fifo_in, "w");
  if (to_cmpi == NULL)
  {
    printf("Could not open: %s \n", fifo_in);
    printf("errno:  %s\n", strerror(errno));
    fflush(stdout);
    return false;
  }
  from_cmpi = fopen(fifo_out, "r");
  if (from_cmpi == NULL)
  {
    printf("Could not open: %s \n", fifo_out);
    printf("errno:  %s\n", strerror(errno));
    fflush(stdout);
    return false;
  }
  return true;
}

/*
char*
driver_access_ok(char* buffer, int length, FILE* file)
{
  char* result;

  result = fgets(buffer, length, file);
  if (!result)
    return NULL;

  if (strcmp(msg, "ok\n"))
  {
    printf("cmpi-cp: put(): received:_ %s %i\n", msg, strlen(msg));
    return CMPI_ERROR_UNKNOWN;
  }
}
*/

void
accessor_fifo_close()
{
  fclose(to_cmpi);
  fclose(from_cmpi);
}

/**
  @param buffer with length 32 or greater.
*/
CMPI_RETURN
read_ok(char* buffer, FILE* file)
{
  buffer[0] = '\0';
  char* tmp = fgets(buffer, 32, file);

  if (tmp == NULL)
    return CMPI_ERROR_UNKNOWN;

  if (strncmp(buffer, "ok", 2) != 0)
  {
    printf("did not get ok\n");
    fflush(stdout);
    printf("got: %s\n", buffer);
    return CMPI_ERROR_UNKNOWN;
  }

  return CMPI_SUCCESS;
}

CMPI_RETURN
accessor_put(char* key, void* data, int length)
{
  char buffer[32];
  CMPI_RETURN error;

  fprintf(to_cmpi, "put %s %i\n", key, length);
  fflush(to_cmpi);

  error = read_ok(buffer, from_cmpi);
  CHECK_ERROR;

  buffer_to_stream(data, length, to_cmpi);
  fflush(to_cmpi);

  read_ok(buffer, from_cmpi);
  CHECK_ERROR;

  return CMPI_SUCCESS;
}
