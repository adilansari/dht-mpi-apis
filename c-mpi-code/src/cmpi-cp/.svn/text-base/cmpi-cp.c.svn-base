
#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <io_tools.h>
#include <accessor.h>

#define CMPI_CP_CHUNK 1024

#include <cmpi-types.h>

#include <cmpi-cp.h>

#ifdef DMALLOC
#include <dmalloc.h>
void setup_dmalloc()
{
  dmalloc_logpath = malloc(64);
  strcpy(dmalloc_logpath, "dmalloc_cmpicp.out");
  // printf("dmalloc_logpath: %s\n", dmalloc_logpath);
}
#else
#define setup_dmalloc()
#endif

char* target1;
char* target2;

int value_size;

bool fifos_open = false;

void
finish(char* msg, int code)
{
  // if (fifos_open)
  // driver_access_fifo_close();
  if (msg)
    printf("%s\n", msg);
#ifdef DMALLOC
  dbg("dmalloc");
  // printf("dmalloc_logpath: %s\n", dmalloc_logpath); \
  dmalloc_shutdown();                                 \
  dmalloc_log_unfreed();
#endif
  exit(code);
}

void
cmpi_cp_args(int argc, char* argv[])
{
  if (argc == 1)
  {
    printhelp();
    finish(NULL, 1);
  }
  target1 = argv[1];
  target2 = argv[2];
}

void
printhelp()
{
  printf("usage: cmpi [OPTION] target1 target2         \n");
  printf("       each target is a file path            \n");
  printf("       or a standard stream -                \n");
  printf("       or a DHT key formatted as dht://<key> \n\n");
  printf("options:                                     \n");
  printf("     -h      print this message              \n");
  printf("     -v      show version information        \n");
  printf("     -c      set chunk size                  \n");
}

/**
   Determine the type of this target, file or key.
*/
CMPI_CP_TYPE
cmpi_cp_extract_type(char* target, char* value)
{
  CMPI_CP_TYPE result;
  assert(target);
  assert(value);
  if (strstr(target, "dht://"))
  {
    strcpy(value, target+6);
    result = CMPI_CP_KEY;
  }
  else if (strstr(target, "-") && strlen(target) == 1)
  {
    strcpy(value, target);
    result = CMPI_CP_STREAM;
  }
  else
  {
    strcpy(value, target);
    result = CMPI_CP_FILE;
  }
  return result;
}

int
cmpi_cp_put(FILE* file, char* object)
{
  char key[CMPI_KEY_LENGTH+16];
  char data[value_size];
  int  count = 0;
  int  total;
  while (! feof(file))
  {
    total = 0;
    while (!feof(file) &&
           total < value_size)
    {
      int chunk  = value_size-total;
      int actual = fread(data+total, 1, chunk, file);
      data[total+actual] = '\0';
      total += actual;
    }
    if (total == 0)
      break;

    sprintf(key, "%s[%i]", object, count);
    accessor_put(key, data, total);
    count++;
  }

  total = sprintf(data, "DHT: %i", count)+1;
  accessor_put(object, data, total);

  // printf("put() complete\n");
  return CMPI_SUCCESS;
}

int
cmpi_cp_get(char* object, FILE* file)
{
  char msg[64];
  char data[value_size];
  int  count, c, n, i;
  int  length;
  char* result;

  dbg("cmpi_cp_get()...");
  fflush(stdout);

  fprintf(to_cmpi, "get %s\n", object);
  fflush(to_cmpi);

  result = NULL;
  i = 0;
  while ((result = fgets(msg, 64, from_cmpi)) == NULL)
  {
    printf("cmpi-cp: get(): bad metadata length: NULL\n");
    if (i++ == 3)
      return CMPI_ERROR_UNKNOWN;
  }
  n = sscanf(msg, "%i", &length);

  if (n != 1)
  {
    printf("cmpi-cp: get(): bad metadata length: %s\n", msg);
    return CMPI_ERROR_UNKNOWN;
  }
  if (length == -1)
    return CMPI_DOESNT_EXIST;

  // printf("metadata length: %i\n", length);
  // fflush(stdout);

  c = stream_to_buffer(data, length, from_cmpi);
  c = sscanf(data, "DHT: %i\n", &count);
  if (c != 1)
  {
    printf("cmpi-cp: get(): bad metadata: %s\n", data);
    finish(NULL, 1);
  }

  // printf("count: %i\n", count);

  for (c = 0; c < count; c++)
  {
    fprintf(to_cmpi, "get %s[%i]\n", object, c);
    fflush(to_cmpi);
    result = fgets(msg, 64, from_cmpi);
    if (!result)
    {
      printf("got bad data length: NULL\n");
      return CMPI_ERROR_UNKNOWN;
    }
    n = sscanf(msg, "%i", &length);
    if (n != 1)
    {
      printf("cmpi-cp: get(): bad data length: %s\n", msg);
    }
    if (length == -1)
    {
      printf("not found: %s[%i]\n", object, c);
      return CMPI_DOESNT_EXIST;
    }

    stream_to_buffer(data, length, from_cmpi);
    buffer_to_stream(data, length, file);
  }

  // printf("cmpi_cp_get(): complete\n");

  return CMPI_SUCCESS;
}

void
bad_file(char* filename)
{
  printf("Could not open: %s\n", filename);
  finish(NULL, 1);
}

/**
   Case logic for different uses
*/
int
cmpi_cp_case(CMPI_CP_TYPE type1, char* object1,
             CMPI_CP_TYPE type2, char* object2)
{
  int result = CMPI_SUCCESS;
  if (type1 == CMPI_CP_FILE &&
      type2 == CMPI_CP_KEY)
  {
    // FILE -> DHT
    FILE* file = fopen(object1, "r");
    if (!file)
      bad_file(object1);
    result = cmpi_cp_put(file, object2);
    fclose(file);
  }
  else if (type1 == CMPI_CP_KEY &&
           type2 == CMPI_CP_FILE)
  {
    // DHT -> FILE
    FILE* file = fopen(object2, "w");
    if (!file)
      bad_file(object2);
    result = cmpi_cp_get(object1, file);
    fclose(file);
  }
  else if (type1 == CMPI_CP_KEY &&
           type2 == CMPI_CP_KEY)
  {
    // DHT -> DHT
  }
  else if (type1 == CMPI_CP_STREAM &&
           type2 == CMPI_CP_KEY)
  {
    // STREAM -> DHT
    result = cmpi_cp_put(stdin, object2);
  }
  else if (type1 == CMPI_CP_KEY &&
           type2 == CMPI_CP_STREAM)
  {
    // DHT -> STREAM
    result = cmpi_cp_get(object1, stdout);
  }
  return result;
}

/**
   Exit codes:
   0 : success
   1 : error
   2 : not found
*/
int
main(int argc, char* argv[])
{
  dbg("\ncmpi-cp start\n");

  // setup_dmalloc();

  CMPI_CP_TYPE target1_type;
  CMPI_CP_TYPE target2_type;

  char object1[CMPI_KEY_LENGTH+10];
  char object2[CMPI_KEY_LENGTH+10];

  value_size = CMPI_CP_CHUNK;

  cmpi_cp_args(argc, argv);
  target1_type = cmpi_cp_extract_type(target1, object1);
  target2_type = cmpi_cp_extract_type(target2, object2);

  if (target1_type == CMPI_CP_FILE &&
      target2_type == CMPI_CP_FILE)
    finish("cmpi-cp: given two files!", 1);

  accessor_fifo_setup(NULL);
  accessor_fifo_init();
  fifos_open = true;
  dbg("access");

  int result =
    cmpi_cp_case(target1_type, object1, target2_type, object2);

  if (result == CMPI_DOESNT_EXIST)
  {
    // cmpi_get could not find the dht:// target
    printf("does not exist: %s\n", target1);
    finish(NULL, 2);
  }
  else if (result != CMPI_SUCCESS)
    finish("error!\n", 1);

  accessor_fifo_close();

  dbg("cmpi-cp: normal exit\n");
  finish(NULL, 0);
}
