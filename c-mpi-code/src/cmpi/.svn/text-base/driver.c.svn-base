
/**
 * Command-line client for CMPI.
 */

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cmpi-driver.h>
#include <list.h>

char* filename = NULL;

bool driver_quitting = false;

char* fifo_in = NULL;
char* fifo_out = NULL;

cmpi_driver*
driver_create(void)
{
  cmpi_driver* driver = malloc(sizeof(cmpi_driver));
  driver->quitting = false;
  return driver;
}

bool
driver_setup_file(cmpi_driver* driver, char* filename)
{
  driver->source = fopen(filename, "r");
  if (driver->source == NULL)
  {
    printf("Could not open: %s \n", filename);
    return false;
  }
  return true;
}

bool
driver_setup_stdin(cmpi_driver* driver)
{
  driver->source = stdin;
  driver->sink   = stdout;
  return true;
}

/**
   Setup driver fifo locations.
   Based on C-MPI parameter driver_dir
*/
void driver_setup_fifo_names()
{
  char* dir = cmpi_params_get("driver_dir");
  if (!dir)
    dir = "/tmp";

  fifo_in = malloc(CMPI_BLOB_LENGTH*sizeof(char));
  fifo_out = malloc(CMPI_BLOB_LENGTH*sizeof(char));

  fifo_in[0] = '\0';
  fifo_out[0] = '\0';

  strcat(fifo_in, dir);
  strcat(fifo_in, "/cmpi.fifo.in");

  strcat(fifo_out, dir);
  strcat(fifo_out, "/cmpi.fifo.out");

  NOTE_FSS(fifo_in,fifo_out);

  signal(SIGPIPE, SIG_IGN);
}

/**
   @return True iff the fifos were set up and opened correctly.
*/
bool
driver_setup_fifo(cmpi_driver* driver)
{
  if (!fifo_in)
    driver_setup_fifo_names();

  int error = mkfifo(fifo_in, S_IRUSR|S_IWUSR);
  if (error != 0 &&
      errno != EEXIST)
  {
    printf("Could not create fifo: %s\n", fifo_in);
    return false;
  }
  NOTE_S("created fifo: ", fifo_in);
  error = mkfifo(fifo_out, S_IRUSR|S_IWUSR);
  if (error != 0 &&
      errno != EEXIST)
  {
    printf("Could not create fifo: %s\n", fifo_in);
    return false;
  }
  NOTE_S("created fifo: ", fifo_out);

  driver->source = fopen(fifo_in, "r");
  if (driver->source == NULL)
  {
    printf("Could not open: %s \n", fifo_in);
    return false;
  }

  driver->sink   = fopen(fifo_out, "w");
  if (driver->sink == NULL)
  {
    printf("Could not open: %s \n", fifo_out);
    return false;
  }

  return true;
}

void
driver_put(cmpi_driver* driver, struct list* tokens)
{
  char* key = (char*) tokens->head->next->data;
  char  value[CMPI_VALUE_LENGTH];
  int   length;
  int   n;
  n = sscanf(tokens->head->next->next->data, "%i", &length);
  assert(n == 1);

  SHOW_FSI(key, length);
  fprintf(driver->sink, "ok got: put-command\n");
  fflush(driver->sink);

  OK;
  NOTE("reading to buffer");
  stream_to_buffer(value, length, driver->source);
  NOTE("got buffer");
  cmpi_put(key, value, length);

  NOTE("put complete\n");

  int error = fprintf(driver->sink, "ok\n");
  if (error == -1)
    printf("driver_put result error!\n");
  fflush(driver->sink);
}

void
driver_get(cmpi_driver* driver, struct list* tokens)
{
  char* key = tokens->head->next->data;

  NOTE_FS(key);

  void* value;
  int length;
  cmpi_get(key, &value, &length);

  if (value == NULL)
  {
    fprintf(driver->sink, "-1\n");
  }
  else
  {
    fprintf(driver->sink, "%i\n", length);
    fflush(driver->sink);
    buffer_to_stream(value, length, driver->sink);
  }
  fflush(driver->sink);
  DONE;
}

void
driver_info(struct list* tokens)
{
  char* p = tokens->head->next->data;
  int i;
  sscanf(p, "%i", &i);
  char* result = cmpi_info(i);
  printf("%s", result);
}

void
driver_lookup(struct list* tokens)
{
  char* p = tokens->head->next->data;
  cmpi_lookup(p);
}

void
driver_sleep(cmpi_driver* driver, struct list* tokens)
{
  char* p = tokens->head->next->data;
  int i;
  sscanf(p, "%i", &i);
  SHOW_FI(i);
  sleep(i);
  fprintf(driver->sink, "ok\n");
  fflush(driver->sink);
  NOTE("awake");
}

void
driver_quit(cmpi_driver* driver)
{
  NOTE_F;

  driver->quitting = true;

  fprintf(driver->sink, "ok\n");
  fflush(driver->sink);

  sleep(1);

  cmpi_shutdown();
  DONE;
}

void
driver_execute(cmpi_driver* driver)
{
  SHOW_FS(driver->command);

  struct list* tokens = list_parse(driver->command);

  NOTE("got tokens: ");
  DEBUG(list_printf("%s", tokens));

  if (tokens->size == 0)
    return;

  char* optoken = tokens->head->data;
  if (strncmp(optoken, "#", 1) == 0)
  {
    ; // This is a comment.
  }
  else if (strncmp(optoken, "put", 3) == 0)
  {
    driver_put(driver, tokens);
  }
  else if (strncmp(optoken, "get", 3) == 0)
  {
    driver_get(driver, tokens);
    NOTE("driver_get returned\n");
  }
  else if (strncmp(optoken, "lookup", 6) == 0)
  {
    driver_lookup(tokens);
  }
  else if (strncmp(optoken, "info", 4) == 0)
  {
    driver_info(tokens);
  }
  else if (strncmp(optoken, "sleep", 5) == 0)
  {
    driver_sleep(driver, tokens);
  }
  else if (strncmp(optoken, "quit", 4) == 0)
  {
    driver_quit(driver);
  }
  list_destroy(tokens);
}

void
driver_process(cmpi_driver* driver)
{
  // gossip_ldebug(MASK_DRIVER, "driver_process()...\n");
  while (! driver->quitting)
  {
    char* result =
      fgets(driver->command, CMPI_DRIVER_MAX_COMMAND, driver->source);
    if (!result)
    {
      NOTE("Resetting driver...");
      fclose(driver->source);
      fclose(driver->sink);
      driver_setup_fifo(driver);
      continue;
    }
    char* s = strchr(driver->command, '\n');
    *s = '\0';
    driver_execute(driver);
    NOTE("cmd complete");
  }
  NOTE("Closing driver...");
  fclose(driver->source);
  fclose(driver->sink);
}

void
dump_info(char* filename)
{
  int i;
  FILE* f = fopen(filename, "w");
  if (! f)
  {
    printf("file error: %s \n", filename);
    return;
  }

  for (i = 1; i < mpi_size; i++)
  {
    char* t = cmpi_info(i);
    fprintf(f, "%s\n", t);
  }
  fclose(f);
}
