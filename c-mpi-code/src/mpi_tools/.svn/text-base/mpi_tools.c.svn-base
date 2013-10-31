
#include "mpi_tools.h"

int mpi_size;
int mpi_rank;

int debug_rank = -1;

char output[100];

bool  debug_multiple = false;
FILE* debug_file     = NULL;

void
setup_debug_file(void)
{
  char debug_path[100];

  // debug_multiple = true;

  if (debug_file == NULL)
  {
    if (debug_multiple)
    {
      if (mpi_rank == 0)
        printf("Using multiple debug files \n");
      sprintf(debug_path, "%s%i.out", "debug", debug_rank);
      debug_file = fopen(debug_path, "w");
    }
    else
    {
      if (mpi_rank == 0)
        DEBUG(puts("Debug to stdout"));
      debug_file = stdout;
    }
  }
}

void whoami()
{
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  if (debug_rank == -1)
    debug_rank = mpi_rank;

  DEBUG(setup_debug_file());
}

/*
void
sync_output(int rank, int size, char* msg)
{
  int max_length;
  int length     = strlen(msg)+1;

  MPI_Reduce(&length, &max_length, 1, MPI_INT, MPI_MAX, 0,
             MPI_COMM_WORLD);

  char *buf;

  if (rank != 0)
  {
    MPI_Send(msg, length, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }
  else
  {
    int i;
    MPI_Status status;
    buf = (char*) malloc(sizeof(char)*max_length);

    for (i = 1; i < size; i++)
    {
      MPI_Recv(buf, max_length, MPI_CHAR, i,
               0, MPI_COMM_WORLD, &status);
      printf("%s\n", buf);
    }
  }
}
*/

/**
   Print possibly binary data with nice handling of newline, tab,
   NULL characters, and misc. non-printing characters
 */
void
fprintdata(FILE* file, char* tag, char* data, int length)
{
  int i;
  int c;
  fprintf(file, "%s[", tag);
  for (i = 0; i < length; i++)
  {
    c = data[i];
    if (c == '\0')
      fprintf(file, "\\0");
    else if (c == '\n')
      fprintf(file, "\\n");
    else if (c == '\t')
      fprintf(file, "\\t");
    else if (c >= 32 && c <=126)
      fprintf(file, "%c", c);
    else
      fprintf(file, "\\%i", (int) c);
  }
  fprintf(file, "]");
}

/**
   Print possibly binary data with nice handling of newline, tab,
   NULL characters, and misc. non-printing characters
 */
void
printdata(char* tag, char* data, int length)
{
  fprintdata(stdout, tag, data, length);
}

/**
   Print the bits of a binary number
 */
void
printbinary(unsigned int number)
{
  int x;
  for (x = 31; x>=0; x--)
  {
    if ((number & (1 << x)) != 0)
      printf("1");
    else
      printf("0");
  }
  printf("\n");
}

/**
   Flip the second bit of the input.
*/
int
flip2(int i)
{
  static int s = 1 << 30;
  return i^s;
}

unsigned int
ilog2(unsigned int j)
{
  unsigned int r = 0;
  while (j >>= 1)
    r++;
  return r;
}

unsigned int
isqrt(unsigned int j)
{
  return (unsigned int) sqrt((double) j);
}

void
note(char* msg)
{
  if (msg == NULL)
    fprintf(debug_file, "[%i] (NULL) \n", debug_rank);
  else
    fprintf(debug_file, "[%i] %s \n", debug_rank, msg);

  fflush(debug_file);
}

void
note_b(char* msg, int b)
{
  fprintf(debug_file, "[%i] %s", debug_rank, msg);
  printbinary((unsigned int) b);
  fflush(debug_file);
}

void
note_d(char* msg, double d)
{
  fprintf(debug_file, "[%i] %s %g\n", debug_rank, msg, d);
  fflush(debug_file);
}

void
note_p(char* msg, void* p)
{
  fprintf(debug_file, "[%i] %s%p\n", debug_rank, msg, p);
  fflush(debug_file);
}

void
note_i(char* msg, int i)
{
  fprintf(debug_file, "[%i] %s%i\n", debug_rank, msg, i);
  fflush(debug_file);
}

void
note_ii(char* msg, int i, int j)
{
  fprintf(debug_file, "[%i] %s(%i,%i)\n", debug_rank, msg, i, j);
  fflush(debug_file);
}

void
note_u(char* msg, unsigned int u)
{
  fprintf(debug_file, "[%i] %s%ui\n", debug_rank, msg, u);
  fflush(debug_file);
}

void
note_xi(char* msg, int i, int j)
{
  fprintf(debug_file, "[%i] %s%X %i\n", debug_rank, msg, i, j);
  fflush(debug_file);
}

void
note_s(char* msg, char* s)
{
  assert(debug_file);
  fprintf(debug_file, "[%i] %s%s\n", debug_rank, msg, s);
  fflush(debug_file);
}

void
note_sx(char* msg, char* s, int i)
{
  fprintf(debug_file, "[%i] %s%s %X\n", debug_rank, msg, s, i);
  fflush(debug_file);
}

void
note_sxi(char* msg, char* s, int x, int i)
{
  fprintf(debug_file, "[%i] %s%s %X %i\n", debug_rank, msg, s, x, i);
  fflush(debug_file);
}

void
note_ss(char* msg, char* s1, char* s2)
{
  fprintf(debug_file, "[%i] %s%s %s", debug_rank, msg, s1, s2);
  fflush(debug_file);
}


/*
void
note_ssxi(char* msg, char* s1, char* s2, int x, int i)
{
  fprintf(debug_file, "[%i] %s%s %X %i\n", debug_rank, msg, s, x, i);
  fflush(debug_file);
}
*/

void
note_x(char* msg, int i)
{
  fprintf(debug_file, "[%i] %s%X\n", debug_rank, msg, i);
  fflush(debug_file);
}

void
note_xx(char* msg, int i, int j)
{
  fprintf(debug_file, "[%i] %s%X\n", debug_rank, msg, i);
  fflush(debug_file);
}

void
show_d(char* tag, double d)
{
  fprintf(debug_file, "[%i] %s: %g\n", debug_rank, tag, d);
  fflush(debug_file);
}

/*
void
show_i(char* tag, int i)
{
  fprintf(debug_file, "[%i] %s: %i\n", debug_rank, tag, i);
  fflush(debug_file);
}
*/

void
show_ii(char* tag_i, int i, char* tag_j, int j)
{
  fprintf(debug_file, "[%i] %s: %i, %s: %i\n", debug_rank, tag_i, i, tag_j, j);
  fflush(debug_file);
}

void
show_l(char* tag, long l)
{
  fprintf(debug_file, "[%i] %s: %li\n", debug_rank, tag, l);
  fflush(debug_file);
}

void
show_u(char* tag, unsigned int u)
{
  fprintf(debug_file, "[%i] %s: %u\n", debug_rank, tag, u);
  fflush(debug_file);
}

/*
void
show_s(char* tag, char* s)
{
  fprintf(debug_file, "[%i] %s: %s\n", debug_rank, tag, s);
  fflush(debug_file);
}
*/

void
show_x(char* tag, int x)
{
  fprintf(debug_file, "[%i] %s: %X\n", debug_rank, tag, x);
  fflush(debug_file);
}

void
show_b(char* tag, int b)
{
  fprintf(debug_file, "[%i] %s is %s\n", debug_rank, tag,
         b ? "true" : "false");
  fflush(debug_file);
}

void
show_p(char* tag, void* p)
{
  fprintf(debug_file, "[%i] %s: %p\n", debug_rank, tag, p);
  fflush(debug_file);
  fflush(stdout);
}

void
timestamp(char* label, char* msg)
{
  fprintf(debug_file, "%s", timestring(label, msg));
}

/**
   @return Formatted time string in static output location.
*/
char*
timestring(char* label, char* msg)
{
  static struct timeval tp;
  gettimeofday(&tp, NULL);
  if (msg)
    sprintf(output, "%s: at %li.%li %s \n",
            label, tp.tv_sec, tp.tv_usec, msg);
  else
    sprintf(output, "%s: at %li.%li \n",
           label, tp.tv_sec, tp.tv_usec);
  return output;
}

int
roll(int n)
{
  return (int) (n * 1.0 *
                (rand() / (RAND_MAX+1.0)));
}

/**
   Copy the string onto the heap so it can be used asynchronously.
   Just like strdup()
   @return A copy of the string or NULL if given NULL.
*/
char*
heap(char* s)
{
  // int length;
  char* h;
  if (s == NULL)
    return NULL;
  h = malloc((strlen(s)+1)*sizeof(char));
  assert(h != NULL);
  strcpy(h, s);
  return h;
}

/**
   Copy the int onto the heap as a decimal string.
*/
char*
iheap(int i)
{
  char* h = (char*) malloc(20*sizeof(char));
  assert(h != NULL);
  sprintf(h, "%i", i);
  return h;
}

/**
   Copy the int onto the heap as a hex string.
*/
char*
xheap(int i)
{
  char* h = (char*) malloc(20*sizeof(char));
  assert(h != NULL);
  sprintf(h, "%X", i);
  return h;
}

/**
   Return a random number in [0,n).
*/
int
rand_lt(int n)
{
  double d = (n*((double) rand())) / RAND_MAX;
  int result = (int) d;
  return result;
}

/**
   Return a random double in [0,n).
*/
double
random_double(int n)
{
  double d = (n*((double) rand())) / RAND_MAX;
  return d;
}

/**
   Read in a whole small file as a string.
*/
char*
file_string(char* file)
{
  intmax_t size;
  struct stat stats;

  stat(file, &stats);
  size = (intmax_t) stats.st_size;
  char* result = malloc(size+1);

  FILE* f = fopen(file, "r");
  int r = fread(result, 1, size, f);
  assert(r == size);
  result[size] = '\0';
  fclose(f);
  return result;
}

#ifdef ENABLE_DEBUG
#ifdef HAVE_EXEC_INFO_H
/**
   Dump the stack and exit.
*/
void
dump_stack()
{
  static void *buffer[100];
  // char **strings;
  int nptrs;
  nptrs = backtrace(buffer, 100);
  fprintf(debug_file, "backtrace() returned %d addresses\n", nptrs);
  fflush(NULL);
  backtrace_symbols_fd(buffer, nptrs, 1);
  exit(1);
}
#else
void
dump_stack()
{
  fprintf(debug_file, "cannot dump stack. \n");
}
#endif
#endif

#ifdef DMALLOC
/**
   Set up DMALLOC output
*/
void
dmalloc_setup(void)
{
  dmalloc_logpath = malloc(100);
  assert(dmalloc_logpath != NULL);
  sprintf(dmalloc_logpath, "./dmalloc_%i.out", debug_rank);
  SHOW_S(dmalloc_logpath);
  fflush(NULL);
}
#endif
