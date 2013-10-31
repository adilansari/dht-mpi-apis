
/**
 * Ignores disk operations.
 * Used if no disk options are given at configure time.
 * */

#include <mpi_tools.h>

void
cmpi_disk_storepair(char* key, char* value, int length)
{
  NOTE_FS(key);
}

int cmpi_disk_loadpair(char* key,  void** value)
{
  NOTE_F;
  *value = NULL;
  return 0;
}
