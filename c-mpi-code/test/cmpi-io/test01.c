
#include <node.h>

#include "test_helpers.h" 

void
cmpi_client_code()
{
  wait_for_notification(); 
  notify_next(); 
  
  NOTE("NOTIFIED"); 
  
  MPI_File file; 
  MPI_File_open(cmpi_comm_clients, "cmpi:/sandbox/file.txt",
                MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL,
                &file);
  sleep(1);
  cmpi_shutdown(); 
}
