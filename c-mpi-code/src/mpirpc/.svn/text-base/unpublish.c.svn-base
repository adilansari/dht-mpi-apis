
/**
   Simply unpublish a name from the MPI-2 namespace. 
*/

#include <stdio.h> 
#include <stdlib.h>

#include <mpi.h>

void
printhelp(void)
{
  printf("usage: unpublish <name> \n");
  printf("returns 0 on success, 1 on error \n"); 
}

void
crash(int value)
{
  MPI_Finalize(); 
  exit(value); 
}

int
main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);

  if (argc != 2 ||
      argv[1][0] == '-')
  {
    printhelp(); 
    crash(1); 
  }

  char* name = argv[1];
  char  port[MPI_MAX_PORT_NAME];
  int   err = MPI_Lookup_name(name, MPI_INFO_NULL, port);
  if (err == MPI_SUCCESS)
  {
    printf("%s found.  Unpublishing... \n", name); 
    MPI_Unpublish_name(name, MPI_INFO_NULL, port); 
  }
  else
  {
    printf("%s not found! \n", name);
    crash(1); 
  }

  MPI_Finalize();

  return 0; 
}
