
#ifndef CMPI_H
#define CMPI_H

#include <cmpi-config.h>

#include <mpi.h>

#include <mpirpc.h>
#include <lru_table.h>
#include <ltable.h>

#ifdef DISKSIM_LOCATION
#include <disksim.h>
#endif

#include <cmpi_disk.h>

#ifdef USE_CMPI_IO
#include <cmpi-io.h>
#endif

#include <keyvalue.h>

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#include <cmpi-types.h>

typedef unsigned int CMPI_ID;

//// Storage settings...

/**
   Number of keys to hold in lru_table cache
   before flushing to disk or dropping.
*/
extern int cmpi_cache_limit;

extern int cmpi_nodes;

#define CMPI_BLOB_LENGTH (4*1024*1024)

extern struct lru_table* cmpi_cache;

//// Communication settings...
// Table size for outstanding asynchronous communication:
#define CALL_TABLE_SIZE 16

extern int cmpi_status;

extern bool cmpi_quitting;

//// Disksim integration...
#ifdef DISKSIM_LOCATION
#define DISKSIM(x) x
#else
#define DISKSIM(x)
#endif

enum
{
  CMPI_MODE_NODE,
  CMPI_MODE_CLIENT
};

enum
{
  CMPI_STATUS_PROTO,   // CMPI is starting up
  CMPI_STATUS_READY,   // CMPI is fully connected
  CMPI_STATUS_SHUTDOWN // CMPI is shutting down
};

enum
{
  CMPI_TAG_REQUEST,  // Request a transmission (unused?)
  CMPI_TAG_ID,       // Send an ID
  CMPI_TAG_CONTROL,  // Send a message
  CMPI_TAG_KEY,      // Send a key
  CMPI_TAG_DATA,     // Send blob
  CMPI_TAG_NODE,     // Send a node number
  CMPI_TAG_ERRCODE   // Send a blob lookup error code
};

/** CMPI control message types.
    Must be associated with normal variables
    to allow MPI transmissions (by reference).
*/
enum
{
  CMPI_MSG_JOIN,      // Join the DHT
  CMPI_MSG_NEIGHBOR,  // Send any neighbor
  CMPI_MSG_SENDNODE,  // Send your DHT node number
  CMPI_MSG_SENDKEY,   // Send a key
  CMPI_MSG_SENDBLOB,  // Send blob
  CMPI_MSG_LOOKUPKEY, // Request a key lookup
  CMPI_MSG_GETBLOB,   // Request a blob
  CMPI_MSG_SHUTDOWN   // Order a shutdown
};

/**
   CMPI control message types.
   Must be in-memory integers to allow referencing by MPI calls.
 */
extern int cmpi_msg_join;
extern int cmpi_msg_neighbor;
extern int cmpi_msg_sendnode;
extern int cmpi_msg_sendkey;
extern int cmpi_msg_sendblob;
extern int cmpi_msg_lookupkey;
extern int cmpi_msg_getblob;
extern int cmpi_msg_shutdown;

CMPI_RETURN cmpi_init(void);
CMPI_RETURN cmpi_init_client(void);

//// Methods to aid passing information to user programs:
void  cmpi_params_init(void);

/**
   Stores a copy of the key, value pair.
*/
void  cmpi_params_add(char* key, char* value);

/**
   Obtain pointer to the result from the params table.
*/
char* cmpi_params_get(char* key);

//// Methods that the implementation must provide:

/**
   Allow the implementation to initialize.
   Called by cmpi_init().
*/
CMPI_RETURN cmpi_init_impl(void);

/**
   Allow the implementation to initialize in client mode.
   Called by cmpi_init_client().
*/
CMPI_RETURN cmpi_init_impl_client(void);

/**
   Some implementations may require an additional user
   call to connect to the DHT.
*/
CMPI_RETURN cmpi_attach(void);

/**
   Set a CMPI implementation defined parameter.
*/
CMPI_RETURN cmpi_impl_parameter(char* name, char* value);

/**
   Not required.
   Allows for use of the simple node program in src/cmpi/node.c .
*/
void cmpi_client_code(void);

/**
   Obtain an info string from the node serving key.
*/
char* cmpi_info(CMPI_ID key);

/**
   Global debugging operation.
   Instruct all nodes to write out their neighbor tables.
*/
void cmpi_debug_tables(void);

/**
   Global debugging operation.
   Instruct all nodes to write out their neighbor table memory size.
*/
void cmpi_debug_table_usage(void);

/**
   Global debugging operation.
   Instruct all nodes to write out their caches.
*/
void cmpi_debug_caches(void);

/**
   Obtain the CMPI_ID for the node serving key.
*/
CMPI_ID cmpi_lookup(char* k);

//// Core functionality...

/**
   Obtain the node serving key.
*/
CMPI_ID cmpi_translate(CMPI_ID key);

/**
   Retrieve a key/value pair from CMPI.
   @param key The key
   @param value Store pointer to value here
   @param length OUT The value length
*/
CMPI_RETURN cmpi_get(char* key, void** value, int* length);

/**
   Store a key/value pair in CMPI.
*/
CMPI_RETURN cmpi_put(char* key, void* value, int length);

/**
   Update byte region in CMPI key/value pair.
*/
CMPI_RETURN cmpi_update(char* key, void* value,
                        int length, int offset);

/**
   Global operation.
   Instruct p random nodes to emulate a fault state.
*/
void cmpi_emulate_faults(int p);

void cmpi_detach(void);

//// Cached disk operations:

int  cmpi_cached_retrieve(char* key, void** data);
void cmpi_cached_store(char* key,  void* data, int length);
void cmpi_cached_update(char* key, void* data,
                        int offset, int length);

/**
   Instruct all nodes to shutdown.  All clients need to call this.
*/
void cmpi_shutdown(void);

void cmpi_assert_impl(CMPI_RETURN code, const char* message,
                      const char* func, const char* file, int line);
#define cmpi_assert(code, message) \
    cmpi_assert_impl(code,message,__func__,__FILE__,__LINE__)

int  cmpi_return_tostring(char* output, CMPI_RETURN code);

void cmpi_cleanup(void);
void cmpi_client_cleanup(void);

#endif

