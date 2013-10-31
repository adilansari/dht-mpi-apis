
/**
   Attempts to open file test.dat .
   Results in one cmpi_get(test.dat), returning NULL.
   Results in one cmpi_put(test.dat).  
*/

#include <mpi.h> 
#include <cmpi-io.h>

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv); 

  whoami(); 
  
  cmpi_init_client();
  CMPI_RETURN result = cmpi_attach();
  if (result == CMPI_ERROR_SERVICENAME)
  {
    printf("Could not connect to DHT! \n");
    MPI_Finalize(); 
    exit(1); 
  }
  
  MPI_File fh;  
  MPI_File_open(MPI_COMM_WORLD, "test.dat", MPI_MODE_CREATE,
                MPI_INFO_NULL, &fh);
  
  printf("test2 done. \n");

  MPI_Finalize(); 
}
