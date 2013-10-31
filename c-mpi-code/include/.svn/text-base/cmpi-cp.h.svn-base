
/**
 * CMPI-CP command-line tool
 *
 * This has to be as small and fast as possible.
 * We do not compile with MPICC or any of our libs.
 * */


typedef enum
{
  CMPI_CP_KEY,
  CMPI_CP_FILE,
  CMPI_CP_STREAM,
  CMPI_CP_TYPE_ERROR,
} CMPI_CP_TYPE;

void printhelp(void);

#define debugging(x)
#define dbg(x) debugging(printf("%s\n", x);     \
                         fflush(stdout);)
