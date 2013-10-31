
#include <cmpi.h>

#include <keyvalue.h>

int cmpi_status;

int               cmpi_cache_limit;
struct lru_table* cmpi_cache = NULL;

/**
   Used to store values from cmpi_impl_parameter().
*/
struct hashtable* cmpi_params = NULL;

int cmpi_msg_join;
int cmpi_msg_neighbor;
int cmpi_msg_sendnode;
int cmpi_msg_sendkey;
int cmpi_msg_sendblob;
int cmpi_msg_lookupkey;
int cmpi_msg_getblob;
int cmpi_msg_shutdown;

/**
   True when quitting
*/
bool cmpi_quitting = false;

void
cmpi_params_init()
{
  // NOTE_F;
  cmpi_params = hashtable_create(10);
}

/**
   Add copy of key/value pair to cmpi_params table.
*/
void
cmpi_params_add(char* key, char* value)
{
  SHOW_FSS(key, value);
  hashtable_add(cmpi_params, strdup(key), strdup(value));
}

/**
   Returns pointer to the matching value or NULL if not found.
*/
char*
cmpi_params_get(char* key)
{
  char* result = hashtable_search(cmpi_params, key);
  SHOW_FSS(key, result);
  return result;
}

void
cmpi_basics(void)
{
  cmpi_status = CMPI_STATUS_PROTO;

  MPIRPC_Init();

  cmpi_msg_join      = CMPI_MSG_JOIN;
  cmpi_msg_neighbor  = CMPI_MSG_NEIGHBOR;
  cmpi_msg_sendnode  = CMPI_MSG_SENDNODE;
  cmpi_msg_sendkey   = CMPI_MSG_SENDKEY;
  cmpi_msg_sendblob  = CMPI_MSG_SENDBLOB;
  cmpi_msg_lookupkey = CMPI_MSG_LOOKUPKEY;
  cmpi_msg_getblob   = CMPI_MSG_GETBLOB;
  cmpi_msg_shutdown  = CMPI_MSG_SHUTDOWN;
}

/**
   Must be called after MPI_Init()
*/
CMPI_RETURN
cmpi_init()
{
  cmpi_basics();

  cmpi_cache = lru_table_create(cmpi_cache_limit, cmpi_cache_limit);

  cmpi_init_impl();

  return 0;
}

CMPI_RETURN
cmpi_init_client(void)
{
  cmpi_basics();
  cmpi_init_impl_client();

#ifdef USE_CMPI_IO
  NOTE("USING CMPI-IO \n");
  cmpi_io_init();
#endif

  return CMPI_SUCCESS;
}

/**
   Obtain pointer to requested data
   @param key Locate the data for this key
   @param data OUT Store data pointer here
   @return length of data, 0 if data is NULL
 */
int
cmpi_cached_retrieve(char* key, void** data)
{
  int result;
  NOTE_F;

  DEBUG(lru_table_fprintdata(debug_file, cmpi_cache));
  struct keyvalue* found = lru_table_search(cmpi_cache, key);

  if (found)
  {
    DEBUG(printdata("found: ", found->data, found->length));
    *data  = found->data;
    SHOW_P(*data);
    result = found->length;
    free(found);
  }
  else
  {
    result = cmpi_disk_loadpair(key, data);

    if (*data != NULL)
    {
      struct keyvalue* kv = lru_table_add(cmpi_cache, key,
                                          *data, result);
      if (kv)
      {
        cmpi_disk_storepair(kv->key, kv->data, kv->length);
        keyvalue_destroy(kv);
      }
    }
    else
    {
      result = 0;
      *data  = NULL;
    }
  }

  return result;
}

/**
 */
void
cmpi_cached_store(char* key, void* data, int length)
{
  struct keyvalue* kv;
  SHOW_FSI(key, length);
  kv = lru_table_add(cmpi_cache, key, data, length);
  if (kv)
  {
    cmpi_disk_storepair(kv->key, kv->data, length);
    NOTE_S("CACHE_DROP: ", kv->key);
    free(kv->key);
    free(kv);
  }
}

/**
   Does not use cmpi_disk.
*/
void
cmpi_cached_update(char* key, void* data, int offset, int length)
{
  SHOW_FSII(key,offset,length);
  printdata("update: ", data, length);
  printf("\n");

  DEBUG(lru_table_printdata(cmpi_cache));

  if (lru_table_search(cmpi_cache, key))
  {
    lru_table_update(cmpi_cache, key, data, offset, length);
  }
  else
  {
    if (offset != 0)
    {
      char* update = malloc(offset+length);
      memset(update, '\0', offset);
      memcpy(update+offset, data, length);
      lru_table_add(cmpi_cache, key, update, length);
    }
    else
    {
      lru_table_add(cmpi_cache, key, data, length);
    }
  }
  DEBUG(lru_table_printdata(cmpi_cache));

  DONE;
}

/**
   Free up our resources.
*/
void
cmpi_cleanup()
{
  // NOTE_F;
  MPIRPC_Finalize();
  // struct keyvalue* kv;
  /*
  while ((kv = lru_table_poll(cmpi_cache)))
  {
    ;// free(kv->key);
    // free(kv);
  }
  */
  lru_table_destroy(cmpi_cache);
  // NOTE("free cmpi_params");
  hashtable_destroy(cmpi_params);
}

/**
   Free up our resources.
*/
void
cmpi_client_cleanup()
{
  // NOTE_F;
  hashtable_destroy(cmpi_params);
  MPIRPC_Finalize();
}

void
cmpi_assert_impl(CMPI_RETURN code, const char* message,
                 const char* func, const char* file, int line)
{
  if (code == CMPI_SUCCESS)
    return;

  char type[64];
  if (message == NULL)
    message = "";
  cmpi_return_tostring(type, code);
  printf("c-mpi: assert failed: %s: %s\n", type, message);
  printf("                  in: %s (%s:%i)\n", func, file, line);
  exit(code);
}

int
cmpi_return_tostring(char* output, CMPI_RETURN code)
{
  int length = 0;
  if (code == CMPI_SUCCESS)
    length = sprintf(output, "CMPI_SUCCESS");
  if (code == CMPI_ERROR_UNKNOWN)
    length = sprintf(output, "CMPI_ERROR_UNKNOWN");
  if (code == CMPI_DOESNT_EXIST)
    length = sprintf(output, "CMPI_DOESNT_EXIST");
  if (code == CMPI_ERROR_FAULT)
    length = sprintf(output, "CMPI_ERROR_FAULT");
  if (code == CMPI_SUCCESS)
    length = sprintf(output, "CMPI_ERROR_SERVICENAME");
  if (code == CMPI_ERROR_NEIGHBORS)
    length = sprintf(output, "CMPI_ERROR_NEIGHBORS");
  else assert(false);

  return length;
}
