
#ifndef MPI_TOOLS
#define MPI_TOOLS

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <mpi.h>

#ifdef ENABLE_DEBUG
#ifdef HAVE_EXEC_INFO_H
#include <execinfo.h>
#endif
#endif

#include <gossip.h>

extern int mpi_rank;
extern int mpi_size;

extern int debug_rank;

extern FILE* debug_file;

/**
   Quick setup
 */
void whoami(void);

char* heap(char* s);
char* iheap(int i);
char* xheap(int i);

/**
   Execute code line or block in rank order.
*/
#define sequential(code)                                                \
  {                                                                     \
    int sequential_i;                                                   \
    for (sequential_i = 0; sequential_i < mpi_size;                     \
         sequential_i++)                                                \
    {                                                                   \
      if (mpi_rank == sequential_i)                                     \
      {                                                                 \
        code;                                                           \
      }                                                                 \
      MPI_Barrier(MPI_COMM_WORLD);                                      \
    }                                                                   \
  }

void printbinary(unsigned int number);
void printdata(char* tag, char* data, int length);
void fprintdata(FILE* file, char* tag, char* data, int length);
int  flip2(int i);
unsigned int ilog2(unsigned int j);
unsigned int isqrt(unsigned int j);

/**
   Return a random number in [0,n).
*/
int rand_lt(int n);

/**
   Return a random double in [0,n).
*/
double random_double(int n);

/**
   Concatenate char* s at char* p, update p to the end.
*/
#define cat(p, s) p += sprintf(p, "%s", s);

#ifdef ENABLE_DEBUG
// #warning "ENABLE_DEBUG"
#define DEBUG(x) x
#else
#define DEBUG(x)
#endif

#ifdef ENABLE_DEBUG
/**
   Dump the stack and exit.
*/
void
dump_stack(void);
#endif

#define OK NOTE("ok")

void setup_debug_file(void);

/**
   Simplify debugging statements.
 */
void note(char* msg);
void note_b(char* msg, int b);
void note_d(char* msg, double d);
void note_i(char* msg, int i);
void note_ii(char* msg, int i, int j);
void note_s(char* msg, char* s);
void note_ss(char* msg, char* s1, char* s2);
void note_p(char* msg, void* p);
void note_u(char* msg, unsigned int u);
void note_x(char* msg, int i);
void note_xi(char* msg, int i, int j);
void note_sx(char* msg, char* s, int i);
void note_sxi(char* msg, char* s, int x, int i);
// void note_ssxi(char* msg, char* s, int x, int i);

void show_b(char* tag, int b);
void show_d(char* tag, double d);
void show_i(char* tag, int i);
void show_ii(char* tag_i, int i, char* tag_j, int j);
void show_l(char* tag, long l);
void show_s(char* tag, char* s);
void show_u(char* tag, unsigned int u);
void show_x(char* tag, int x);
void show_p(char* tag, void* p);

void  timestamp(char* label, char* msg);
char* timestring(char* label, char* msg);

#define NOTE(m)                DEBUG(note(m))
#define NOTE_B(m,b)            DEBUG(note_b(m,b))
#define NOTE_D(m,d)            DEBUG(note_d(m,d))
#define NOTE_I(m,i)            DEBUG(note_i(m,i))
#define NOTE_II(m,i,j)         DEBUG(note_ii(m,i,j))
#define NOTE_U(m,u)            DEBUG(note_u(m,u))
#define NOTE_P(m,s)            DEBUG(note_p(m,))
#define NOTE_S(m,s)            DEBUG(note_s(m,s))
#define NOTE_SX(m,s,i)         DEBUG(note_sx(m,s,i))
#define NOTE_SXI(m,s,x,i)      DEBUG(note_sxi(m,s,x,i))
// #define NOTE_SSXI(m,s1,s2,x,i) DEBUG(note_ssxi(m,s1,s2,x,i))
#define NOTE_X(m,i)            DEBUG(note_x(m,i))
#define NOTE_XI(m,i,j)         DEBUG(note_xi(m,i,j))

#define NOTE_F DEBUG(fprintf(debug_file, "[%i] %s()... \n",     \
                             debug_rank, __func__);             \
                     fflush(debug_file);)
#define DONE DEBUG(fprintf(debug_file, "[%i] %s() done. \n",     \
                          debug_rank, __func__);        \
                   fflush(debug_file);)
#define NOTE_FI(i)                              \
  DEBUG(fprintf(debug_file, "[%i] %s(%i)... \n",             \
               debug_rank, __func__, i);        \
        fflush(stdout);)
#define NOTE_FX(i) DEBUG(fprintf(debug_file, "[%i] %s(%X)... \n",            \
                                debug_rank, __func__, i);       \
                         fflush(stdout);)
#define NOTE_FIS(i,s) DEBUG(fprintf(debug_file, "[%i] %s(%i,%s)... \n",      \
                                   debug_rank, __func__, i, s); \
                            fflush(stdout);)
#define NOTE_FXIS(x,i,s)                        \
  DEBUG(fprintf(debug_file, "[%i] %s(%X,%i,%s)... \n",       \
               debug_rank, __func__, x, i, s);  \
        fflush(stdout);)
#define NOTE_FXI(i,j)                           \
  DEBUG(fprintf(debug_file, "[%i] %s(%X,%i)... \n",          \
               debug_rank, __func__, i, j);     \
        fflush(stdout);)
#define NOTE_FXS(i,s)                           \
  DEBUG(fprintf(debug_file, "[%i] %s(%X,%s)... \n",          \
               debug_rank, __func__, i, s);     \
        fflush(stdout););
#define NOTE_FXX(i,j) DEBUG(fprintf(debug_file, "[%i] %s(%X,%X)... \n",      \
                                   debug_rank, __func__, i, j); \
                            fflush(stdout);)
#define NOTE_FS(s) DEBUG(fprintf(debug_file, "[%i] %s(%s)... \n",            \
                                debug_rank, __func__, s);       \
                         fflush(stdout);)
#define NOTE_FSS(s1,s2)                         \
  DEBUG(fprintf(debug_file, "[%i] %s(%s,%s)... \n",          \
               debug_rank, __func__, s1, s2);   \
        fflush(stdout);)
#define NOTE_FSSS(s1,s2,s3)                     \
  DEBUG(fprintf(debug_file, "[%i] %s(%s,%s,%s)... \n",       \
               debug_rank, __func__,            \
               s1, s2, s3);                     \
        fflush(stdout);)
#define NOTE_FSSSI(s1,s2,s3,i)                          \
  DEBUG(fprintf(debug_file, "[%i] %s(%s,%s,%s,%i)... \n",            \
               debug_rank, __func__, s1, s2, s3, i);    \
        fflush(stdout);)

///// Simple variable debugging...
#define SHOW_B(b)    DEBUG(show_b(#b, b))
#define SHOW_D(d)    DEBUG(show_d(#d, d))
#define show_i(i)    note_i(#i ": ", i)
#define SHOW_I(i)    DEBUG(show_i(i))
#define SHOW_II(i,j) DEBUG(show_ii(#i, i, #j, j))
#define SHOW_L(l)    DEBUG(show_l(#l, l))
#define SHOW_U(u)    DEBUG(show_u(#u, u))
#define show_s(s)    note_s(#s ": ", s)
#define SHOW_S(s)    DEBUG(show_s(s))
#define SHOW_X(x)    DEBUG(show_x(#x, x))
#define SHOW_P(p)    DEBUG(show_p(#p, p))
#define SHOW         SHOW_S

//// Functions...
#define show_fi(i)                              \
       fprintf(debug_file, "[%i] %s(%s=%i)... \n",          \
               debug_rank, __func__, #i, i);    \
        fflush(stdout);
#define SHOW_FI(i) DEBUG(show_fi(i))
#define SHOW_FIS(i,s)                                   \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s=%s)... \n",            \
               debug_rank, __func__, #i, i, #s, s);     \
        fflush(stdout);)
#define SHOW_FISS(i,s1,s2)                                      \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s=%s,%s=%s)... \n",              \
               debug_rank, __func__, #i, i, #s1, s1, #s2, s2);  \
        fflush(stdout);)
#define SHOW_FISX(i,s,x)                                                \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s=%s,%s=%X)... \n",         \
                debug_rank, __func__, #i, i, #s, s, #x, x);             \
        fflush(stdout);)

#define show_fs(s) fprintf(debug_file,           \
                           "[%i] %s(%s=%s)... \n",          \
               debug_rank, __func__, #s, s);    \
        fflush(stdout);
#define SHOW_FS(s)                              \
  DEBUG(show_fs(s))

#define SHOW_FX(x)                                \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%X)... \n",            \
               debug_rank, __func__, #x, x);      \
        fflush(stdout);)
#define SHOW_FII(i1,i2)                                     \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s=%i)... \n",                \
               debug_rank, __func__, #i1, i1, #i2, i2);     \
        fflush(stdout);)

#define show_fiii(i1,i2,i3)                              \
      fprintf(debug_file, "[%i] %s(%s=%i,%s=%i,%s=%i)... \n",  \
              debug_rank, __func__, #i1, i1, #i2, i2, #i3, i3);   \
        fflush(stdout);
#define SHOW_FIII(i1,i2,i3) DEBUG(show_fiii(i1,i2,i3))

#define show_fsi(s,i)  \
      fprintf(debug_file, "[%i] %s(%s=%s,%s=%i)... \n",  \
                debug_rank, __func__, #s, s, #i, i);     \
        fflush(stdout);
#define SHOW_FSI(s,i) DEBUG(show_fsi(s,i))

#define show_fsii(s,i1,i2)                                     \
  fprintf(debug_file, "[%i] %s(%s=%s,%s=%i,%s=%i)... \n",      \
              debug_rank, __func__, #s, s, #i1, i1, #i2, i2);  \
  fflush(stdout);
#define SHOW_FSII(s,i1,i2) DEBUG(show_fsii(s,i1,i2))

#define SHOW_FXI(x,i)                                   \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%X,%s=%i)... \n",            \
               debug_rank, __func__, #x, x, #i, i);     \
        fflush(stdout);)

#define SHOW_FXIS(x,i,s)                                \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%X,%s=%i,%s=%s)... \n",      \
               debug_rank, __func__,                    \
               #x, x, #i, i, #s, s);                    \
        fflush(stdout);)

#define SHOW_FXIIS(x,i1,i2,s)                                \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%X,%s=%i,%s=%i,%s=%s)... \n",      \
               debug_rank, __func__,                    \
               #x, x, #i1, i1, #i2, i2, #s, s);                    \
        fflush(stdout);)

#define SHOW_FSS(s1,s2)                                 \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%s,%s=%s)... \n",            \
               debug_rank, __func__, #s1, s1, #s2, s2); \
        fflush(stdout);)
#define SHOW_FSSI(s1,s2,i)                                             \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%s,%s=%s,%s=%i)... \n",        \
                debug_rank, __func__, #s1, s1, #s2, s2, #i, i);        \
        fflush(stdout);)
#define SHOW_FISSI(i1,s1,s2,i2)                                \
  DEBUG(fprintf(debug_file,                                        \
                "[%i] %s(%s=%i,%s=%s,%s=%s,%s=%i)... \n",    \
                debug_rank, __func__, #i1, i1, #s1, s1,   \
                #s2, s2, #i2, i2); \
        fflush(stdout);)
#define SHOW_FSISSI(s1,i1,s2,s3,i2)                                \
  DEBUG(fprintf(debug_file,                                        \
                "[%i] %s(%s=%s,%s=%i,%s=%s,%s=%s,%s=%i)... \n",    \
                debug_rank, __func__, #s1, s1, #i1, i1, #s2, s2,   \
                #s3, s3, #i2, i2); \
        fflush(stdout);)
#define SHOW_FP(p)                              \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%p)... \n",          \
               debug_rank, __func__, #p, p);    \
        fflush(stdout);)

//// Functions with return values...

#define SHOW_FS_X(s,x)                                                  \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%s) -> %X... \n",               \
                debug_rank, __func__, #s, s, x);                        \
        fflush(stdout);)
#define SHOW_FIFS_S(i,f,s1,s2)                                       \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s(%s) -> %s)... \n",     \
                debug_rank, __func__, #i, i, f, s1, s2);             \
        fflush(stdout);)
#define SHOW_FIS_X(i,s,x)                                             \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s=%s) -> %X... \n",       \
                debug_rank, __func__, #i, i, #s, s, x);               \
        fflush(stdout);)
#define SHOW_FXI_B(x,i,b)                                               \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%X,%s=%i) -> %s... \n",         \
                debug_rank, __func__,                                   \
                #x, x, #i, i, b ? "true" : "false");                    \
        fflush(stdout);)


//// Functions of functions...

#define SHOW_FIF(i, f)                               \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s())... \n",           \
               debug_rank, __func__, #i, i, f);       \
        fflush(stdout);)
#define SHOW_FIFS(i, f, s)                             \
  DEBUG(fprintf(debug_file, "[%i] %s(%s=%i,%s(%s))... \n",           \
               debug_rank, __func__, #i, i, f, s);      \
        fflush(stdout);)

#define TIMESTAMP(m) note_i(m, time());

#define DUMPSTACK DEBUG(dump_stack());

char* file_string(char* file);

//// Memory usage debugging:
#ifdef DMALLOC
#include <dmalloc.h>
#define DMALLOC_SETUP(x) x
void dmalloc_setup(void);
#else
#define DMALLOC_SETUP(x)
#define dmalloc_setup(x)
#endif

#define DMALLOC_COMPLETE                              \
  DMALLOC_SETUP(                                      \
    {                                                 \
      SHOW_S(dmalloc_logpath);                        \
      dmalloc_shutdown();                             \
      dmalloc_log_unfreed();                          \
    });

/**
   Return a number in [0,n-1].
*/
int roll(int n);

#endif
