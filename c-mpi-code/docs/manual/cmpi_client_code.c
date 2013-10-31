
#include <cmpi.h>

cmpi_client_code()
{
  sprintf(key,   "key_%i",   mpi_rank);
  sprintf(value, "value_%i", mpi_rank);
  cmpi_put(key, value, strlen(value)+1);

  rank = (mpi_rank+1)%mpi_size;
  sprintf(key, "key_%i", rank);
  cmpi_get(key, &result, &length);
  printf("result(%i): %s\n", length, result);
}
