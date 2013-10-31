
#include <io_tools.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

/**
   @return The total number of bytes stored in buffer.
*/
int
stream_to_buffer(char* buffer, int length, FILE* stream)
{
  int total = 0;

  while (total < length)
  {
    int chunk  = length-total;
    int actual = fread(buffer+total, 1, chunk, stream);
    total += actual;
  }

  return total;
}

void
buffer_to_stream(char* buffer, int length, FILE* stream)
{
  int total = 0;

  while (total < length)
  {
    int chunk  = length-total;
    int actual = fwrite(buffer+total, 1, chunk, stream);
    total += actual;
  }
}

/**
   @param name The file name
   @return Number of bytes in buffer
*/
int
file_to_buffer(char** buffer, char* name)
{
  struct stat stats;
  int error = stat(name, &stats);
  if (error)
    return -1;
  int length = stats.st_size;

  FILE* file = fopen(name, "r");
  if (!file)
  {
    return -1;
  }

  *buffer = malloc(length);

  int total = 0;

  while (total < length)
  {
    int chunk  = length-total;
    int actual = fread((*buffer)+total, 1, chunk, file);
    total += actual;
  }

  return total;
}


void
eat_whitespace(FILE* stream)
{
  char c;
  do
  {
    c = fgetc(stream);
  } while (c == ' ' || c == '\n');
  ungetc(c, stream);
}
