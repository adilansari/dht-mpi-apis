
/**
 * Dumps disk operations to the screen.
 * Used if configure --with-disk-printf is used.
 * */

#include <stdio.h>

#include <mpi_tools.h>

void
cmpi_disk_storepair(char* key, void* value)
{
  NOTE_F;
  printf("DISK_STOREPAIR: %s\n", key);
}

void cmpi_disk_loadpair(char* key, void** value)
{
  printf("DISK_LOADPAIR: %s\n", key);
}
