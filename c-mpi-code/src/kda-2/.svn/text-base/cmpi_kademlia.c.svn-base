
#include "cmpi_kda-2.h"

/**
   List of KDA_Neighbor*s.
*/
struct list* contacts;

static void cmpi_impl_parameters(void);

CMPI_RETURN
cmpi_init_impl()
{
  // NOTE_F;
  cmpi_impl_parameters();
  KDA_Init(3, 3);

  return CMPI_SUCCESS;
}

CMPI_RETURN
cmpi_init_impl_client()
{
  NOTE_F;

  contacts = list_create();
  KDA_Init_client(3,3);

  KDA_Attach(contacts);

  return CMPI_SUCCESS;
}

CMPI_RETURN
cmpi_impl_parameter(char* name, char* value)
{
  if (strcmp(name, "nodes") == 0)
  {
    int n;
    sscanf(value, "%i", &n);
    kda_nodes = n;
  }

  return CMPI_SUCCESS;
}

static void
cmpi_impl_parameters()
{
  char* value = cmpi_params_get("nodes");
  assert(value);
  cmpi_impl_parameter("nodes", value);
}

CMPI_RETURN
cmpi_attach()
{
  /*
  CMPI_RETURN result;
  if (KDA_Attach())
  {
    result = CMPI_SUCCESS;
  }
  else
    result = CMPI_ERROR_SERVICENAME;

  return result;
  */
  printf("cmpi_attach(): DEPRECATED!\n");
  return CMPI_SUCCESS;
}

void
add_contacts()
{
  NOTE_F;
  for (int i = 0; i < KDA_SPACE_SIZE; i++)
    for (struct list_item* item = k_bucket[i]->head;
         item; item = item->next)
      {
	KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
	KDA_Neighbor_dump(neighbor);
	list_add_unique(contacts, KDA_Neighbor_cmp, neighbor);
	KDA_Comm_set_name(neighbor);
      }
}

/**
   If the neighbor is unknown, attach, copy and store in contacts list.
   @param Neighbor to find via id
   @return Internal copy of neighbor
 */
KDA_Neighbor*
contact_lookup(KDA_Neighbor* neighbor)
{
  NOTE_FX(neighbor->id);

  KDA_Neighbor* copy =
      list_select_one(contacts, KDA_Neighbor_cmp, neighbor);
  if (copy)
    return copy;

  puts("cloneing");
  copy = KDA_Neighbor_clone(neighbor);
  KDA_Attach_to(copy, contacts);
  add_contacts();
  return copy;
}

char*
cmpi_info(CMPI_ID key)
{
  char*         result   = NULL;
  KDA_Neighbor* neighbor = (KDA_Neighbor*) list_random(contacts);
  MPIRPC*       rpc      = MPIRPC_Call(neighbor->node, "info",
                                       xheap(key),
                                       "", MPIRPC_PROCEED_NULL);
  MPIRPC_Wait(rpc);
  result = rpc->result;
  MPIRPC_Free(rpc);
  return result;
}

/**
   Ex-DHT search.
   @return The MPI rank closest to the given key.
*/
CMPI_ID
cmpi_lookup(char* key)
{
  NOTE_FS(key);

  KDA_Neighbor* n = (KDA_Neighbor*) list_random(contacts);

  MPIRPC* rpc = KDA_Lookup(n->node, key);

  MPIRPC_Wait(rpc);

  printf("cmpi_lookup()@%X -> %s \n", n->id, (char*) rpc->result);

  return 0;
}

CMPI_ID
cmpi_translate(CMPI_ID id)
{
  NOTE_F;

  NOTE_XI("TRANSLATE: ", id, id);

  MPIRPC_Node* node = (MPIRPC_Node*) list_random(contacts);
  if (node == NULL)
    return CMPI_ERROR_NEIGHBORS;
  NOTE_I("contacts size: ", contacts->size);

  MPIRPC* rpc = KDA_Translate(*node, (KDA_ID) id);
  MPIRPC_Wait(rpc);
  int result = 0;
  sscanf(rpc->result, "%X", &result);
  return result;
}

CMPI_RETURN
cmpi_put(char* key, void* value, int length)
{
  NOTE_FS(key);

  // printf("contacts: %i \n", contacts);

  KDA_Neighbor* n = (KDA_Neighbor*) list_random(contacts);
  char tmp[64];
  KDA_Neighbor_sprint(tmp, n);

  MPIRPC* lookup = KDA_Lookup_k(n->node, key);
  MPIRPC_Wait(lookup);
  struct list* neighbors = KDA_Neighbor_node_list(lookup->result);
  free(lookup->result);
  MPIRPC_Free(lookup);

  for (struct list_item* item = neighbors->head; item;
       item = item->next)
  {
    KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
    DEBUG(KDA_Neighbor_sprint(tmp, neighbor);
          printf("neighbor: %s\n", tmp););
    MPIRPC* rpc = KDA_Store(neighbor, key, value, length);
    MPIRPC_Wait(rpc);
    MPIRPC_Free(rpc);
  }

  list_destroy(neighbors);

  DONE;
  return CMPI_SUCCESS;
}

CMPI_RETURN
cmpi_get(char* key, void** value, int* length)
{
  NOTE_FS(key);

  KDA_Neighbor* n = (KDA_Neighbor*) list_random(contacts);

  MPIRPC* lookup = KDA_Lookup_k(n->node, key);
  MPIRPC_Wait(lookup);
  struct list* neighbors = KDA_Neighbor_node_list(lookup->result);
  free(lookup->result);
  MPIRPC_Free(lookup);
  SHOW_I(neighbors->size);
  if (neighbors->size == 0)
    return CMPI_ERROR_NEIGHBORS;

  for (struct list_item* item = neighbors->head; item;
       item = item->next)
  {
    KDA_Neighbor* neighbor_found = item->data;
    DEBUG(puts("retrieve from: ");
          KDA_Neighbor_dump(neighbor_found););
    KDA_Neighbor* neighbor = contact_lookup(neighbor_found);
    DEBUG(KDA_Neighbor_dump(neighbor));
    MPIRPC* rpc = KDA_Retrieve(neighbor, key);
    MPIRPC_Wait(rpc);
    *value = rpc->result;
    *length = rpc->result_length;
    NOTE_S("value: ", *value);
    MPIRPC_Free(rpc);
    // Assume first retrieval is successful:
    break;
  }

  list_destroy(neighbors);

  NOTE("cmpi_get() done \n");
  return CMPI_SUCCESS;
}

CMPI_RETURN
cmpi_update(char* key, void* value, int length, int offset)
{
  NOTE_FS(key);

  // printf("contacts: %i \n", contacts);

  KDA_Neighbor* n = (KDA_Neighbor*) list_random(contacts);
  MPIRPC* lookup = KDA_Lookup_k(n->node, key);
  MPIRPC_Wait(lookup);
  struct list* neighbors = KDA_Neighbor_node_list(lookup->result);
  MPIRPC_Free(lookup);

  for (struct list_item* item = neighbors->head; item;
       item = item->next)
  {
    KDA_Neighbor* neighbor = (KDA_Neighbor*) item->data;
    MPIRPC* rpc = KDA_Update(neighbor, key, value, length, offset);
    MPIRPC_Wait(rpc);
    MPIRPC_Free(rpc);
  }

  list_destroy(neighbors);

  DONE;
  return CMPI_SUCCESS;
}

void
cmpi_debug_tables()
{
  puts("debug_tables()");
}

void
cmpi_debug_caches()
{
  puts("debug_caches()");
}

void
cmpi_detach()
{
  while (contacts->size > 0)
  {
    NOTE("LOOP");
    KDA_Neighbor* neighbor = (KDA_Neighbor*) list_poll(contacts);
    KDA_Detach(neighbor->node);
    free(neighbor);
  }

  KDA_Detach_hubs();
  NOTE("DETACHED");
}

static void free_contacts();

void
cmpi_shutdown()
{
  KDA_Shutdown(contacts);
  free_contacts();
}

void
free_contacts()
{
  list_destroy(contacts);
}
