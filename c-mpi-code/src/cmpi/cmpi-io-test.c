
#include <cmpi.h> 

/**
   Debugging only.
*/
int
main(int argc, char* argv[])
{
  int i,j;
  MPI_Init(&argc, &argv);

  whoami(); 
  
  MPI_File mpi_file; 
  MPI_Status status; 

  snooze_max = 1000; 
  cmpi_init(); 

  if (mpi_rank == 0)
  {
    sleep(3); 
    
    MPI_File_open(MPI_COMM_WORLD, "/sandbox/mpi-io.dat",
                  MPI_MODE_CREATE|
                  MPI_MODE_RDWR,
                  MPI_INFO_NULL, &mpi_file);
    
    char data[60];
    char* p = data; 
    for (i = 0; i < 6; i++)
    {
      for (j = 0; j < 9; j++)
      {
        p += sprintf(p, "%i", j);
      }
      p += sprintf(p, "X");
    }

    MPI_File_write(mpi_file, data, 60, MPI_CHAR, &status); 

    if (MPI_File_close(&mpi_file) == MPI_SUCCESS)
      NOTE("closed");
    else
      NOTE("close error"); 
    
    cmpi_shutdown(); 
  }

  MPI_Finalize(); 
}
