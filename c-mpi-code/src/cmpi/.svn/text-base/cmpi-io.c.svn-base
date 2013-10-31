
#include "cmpi-io.h" 

bool cmpi_io_ready = false;

/**
   List of CMPI_Files.
*/
struct list* open_files    = NULL; 

CMPI_File CMPI_FILE_NULL   = (CMPI_File) -1; 

void
cmpi_io_init(void)
{
  NOTE_F; 
  open_files = list_create();
  cmpi_io_ready = true;

  /* 
  char* preload = malloc(1024*sizeof(char));
  sprintf(preload, "%s%s/lib/libmpich.so %s/lib/libcmpi-io.so", 
          "LD_PRELOAD=", "MPI_LOCATION", getenv("PWD"));
  NOTE_S("preload: ", preload); 
  putenv(preload);
  */
}

int
CMPI_File_open()
{
  
}

int
MPI_File_open(MPI_Comm comm, char* filename,
              int mode, MPI_Info info, MPI_File* fh)
{
  NOTE_FS(filename);

  char* data = NULL; 
  cmpi_get(filename, &data);
  NOTE_S("data: ", data);

  if (data == NULL)
  {
    if (mode & MPI_MODE_CREATE)
    {
      cmpi_put(filename, "FILE: 0"); 
    }
    else
    {
      return MPI_ERR_NO_SUCH_FILE; 
    }
  }
  
  CMPI_File new_fh = (CMPI_File) malloc(sizeof(CMPI_File));
  new_fh->filename = filename; 
  
  list_add(open_files, new_fh); 
  *fh = (MPI_File) new_fh; 
  
  return MPI_SUCCESS; 
}

// rename to CMPI_File_close() 
int
MPI_File_close(MPI_File* fh)
{
  NOTE_F;

  // MPI_MODE_DELETE_ON_CLOSE
  // MPI_FILE_SYNC

  if (list_remove(open_files, *fh))
  {
    *fh = (MPI_File) CMPI_FILE_NULL; 
    return MPI_SUCCESS;
  }
  else
    return MPI_ERR_FILE; 
}

/**
   Currently assuming count is a multiple of CMPI_IO_BLOCK_SIZE.
*/ 
int
MPI_File_write(MPI_File fh, void* buf, int count,
               MPI_Datatype datatype, MPI_Status* status)
{
  int i;
  NOTE_S("WRITE: ", buf);
  int size = CMPI_IO_BLOCK_SIZE; 
  int blocks = count / size; 

  char name[1024];
  strcpy(name, ((CMPI_File) fh)->filename);
  char *p = name + strlen(name);
  char data[size+1]; 
  for (i = 0; i < blocks; i++)
  {
    sprintf(p, "[%i]%c", i, '\0'); 
    memcpy(data, buf+(i*size), size);
    data[size] = '\0'; 
    cmpi_put(name, data);
    NOTE("put"); 
  }
  // MPI_Status; 
  return MPI_SUCCESS; 
}

int
MPI_File_read(MPI_File fh, void* buf, int count,
              MPI_Datatype datatype, MPI_Status* status)
{
  int i;
  NOTE("READ: ");

  int size = CMPI_IO_BLOCK_SIZE; 
  int blocks = count / size; 

  char name[1024];
  strcpy(name, ((CMPI_File) fh)->filename);
  char *p = name + strlen(name);
  char *data; 
  for (i = 0; i < blocks; i++)
  {
    sprintf(p, "[%i]%c", i, '\0');
    cmpi_get(name, &data);
    memcpy(data, buf+(i*size), size);
  }
  
  return MPI_SUCCESS; 
}

int
MPI_File_delete(char *filename, MPI_Info info)
{
  return MPI_SUCCESS; 
}
