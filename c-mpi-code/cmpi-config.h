/* cmpi-config.h.  Generated from cmpi-config.h.in by configure.  */
/* cmpi-config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* major version number */
#define CMPI_VERSION_MAJOR 0

/* minor version number */
#define CMPI_VERSION_MINOR 1

/* sub version number */
#define CMPI_VERSION_SUB 1

/* DISKSIM Location */
/* #undef DISKSIM_LOCATION */

/* Define to 1 if you have the `bzero' function. */
#define HAVE_BZERO 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define to 1 if you have the `gethostname' function. */
#define HAVE_GETHOSTNAME 1

/* Define to 1 if you have the `gettimeofday' function. */
#define HAVE_GETTIMEOFDAY 1

/* Define to 1 if you have the `index' function. */
#define HAVE_INDEX 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `crypto' library (-lcrypto). */
#define HAVE_LIBCRYPTO 1

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if your system has a GNU libc compatible `malloc' function, and
   to 0 otherwise. */
#define HAVE_MALLOC 1

/* Define to 1 if you have the <malloc.h> header file. */
#define HAVE_MALLOC_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 if you have the `mkfifo' function. */
#define HAVE_MKFIFO 1

/* Use the MPE profiling library */
/* #undef HAVE_MPE */

/* Define to 1 if your system has a GNU libc compatible `realloc' function,
   and to 0 otherwise. */
#define HAVE_REALLOC 1

/* Define to 1 if you have the `socket' function. */
#define HAVE_SOCKET 1

/* Define to 1 if you have the `sqrt' function. */
/* #undef HAVE_SQRT */

/* Define to 1 if you have the `stat' function. */
#define HAVE_STAT 1

/* Define to 1 if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define to 1 if stdbool.h conforms to C99. */
#define HAVE_STDBOOL_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the `strdup' function. */
#define HAVE_STRDUP 1

/* Define to 1 if you have the `strftime' function. */
#define HAVE_STRFTIME 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strnlen' function. */
#define HAVE_STRNLEN 1

/* Define to 1 if you have the `strstr' function. */
#define HAVE_STRSTR 1

/* Define to 1 if you have the `strtoull' function. */
#define HAVE_STRTOULL 1

/* Define if sysinfo.h is present */
#define HAVE_SYSINFO 1

/* Define to 1 if you have the <syslog.h> header file. */
#define HAVE_SYSLOG_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/time.h> header file. */
#define HAVE_SYS_TIME_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "wozniak@mcs.anl.gov"

/* Define to the full name of this package. */
#define PACKAGE_NAME "CMPI"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "CMPI 0.1.1"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "cmpi"

/* Define to the home page for this package. */
#define PACKAGE_URL "none"

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.1.1"

/* Define as the return type of signal handlers (`int' or `void'). */
#define RETSIGTYPE void

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if you can safely include both <sys/time.h> and <time.h>. */
#define TIME_WITH_SYS_TIME 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Using CMPI-IO */
/* #undef USE_CMPI_IO */

/* Not using MPI_COMM_WORLD */
#define USE_COMM_WORLD 1

/* Use cmpi_disk_printf.c */
/* #undef USE_DISK_PRINTF */

/* Use cmpi_disk_void.c */
#define USE_DISK_VOID /**/

/* Using CMPI driver */
/* #undef USE_DRIVER */

/* Include strings.h */
/* #undef USE_STRINGS_H */

/* Using DENSE-1 */
#define USE_TABLE_DENSE_1 1

/* Using Kademlia-2A */
/* #undef USE_TABLE_KDA_2A */

/* Using Kademlia-2B */
/* #undef USE_TABLE_KDA_2B */

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define for Solaris 2.5.1 so the uint64_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT64_T */

/* Define to empty if `const' does not conform to ANSI C. */
/* #undef const */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif

/* Define to rpl_malloc if the replacement function should be used. */
/* #undef malloc */

/* Define to rpl_realloc if the replacement function should be used. */
/* #undef realloc */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */

/* Define to the type of an unsigned integer type of width exactly 64 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint64_t */
