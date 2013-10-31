
#include <stdlib.h>

#include "keyvalue.h"

/**
   Free this keyvalue pair and its member data pointers.
*/
void
keyvalue_destroy(struct keyvalue* kv)
{
  NOTE_F;
  assert(kv);
  free(kv->key);
  free(kv->data);
  free(kv);
}
