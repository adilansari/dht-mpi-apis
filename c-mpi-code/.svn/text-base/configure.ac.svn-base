dnl **************************************************************
dnl CMPI AUTOCONF SCRIPT
dnl
dnl Process this file with autoconf to produce a configure script.
dnl You may need to use autoheader as well if changing any DEFINEs

dnl sanity checks, output header, location of scripts used here
AC_INIT(CMPI, 0.1.1, wozniak@mcs.anl.gov, cmpi, none)

AC_PREREQ(2.59)
AC_CONFIG_AUX_DIR(maint/config)

CONFIGURE_TIME=`date -u +"%Y-%m-%d-%H%M%S"`
CMPI_MAJOR=0
CMPI_MINOR=1
CMPI_SUB=1
CMPI_PRE="pre1-$CONFIGURE_TIME"

CMPI_VERSION=$CMPI_MAJOR.$CMPI_MINOR.$CMPI_SUB$CMPI_PRE
AC_SUBST(CMPI_VERSION)
AC_DEFINE_UNQUOTED(CMPI_VERSION_MAJOR, $CMPI_MAJOR, major version number)
AC_DEFINE_UNQUOTED(CMPI_VERSION_MINOR, $CMPI_MINOR, minor version number)
AC_DEFINE_UNQUOTED(CMPI_VERSION_SUB,   $CMPI_SUB, sub version number)

dnl Checks for host type
dnl Ensures we can compile on this OS
AC_CANONICAL_HOST

if test "$build_cpu" = "powerpc64"; then
   AC_DEFINE(USE_STRINGS_H, 1, [Include strings.h])
fi

USR_CFLAGS=$CFLAGS
if test "x$USR_CFLAGS" = "x"; then
	USR_CFLAGS_SET=no
fi

AC_CONFIG_HEADER(cmpi-config.h)

dnl C configuration

AC_C_BIGENDIAN
AC_C_CONST
AC_C_INLINE

AC_STRUCT_TM

AC_TYPE_SIGNAL

dnl AC_TYPE_INT32_T
dnl AC_TYPE_INT64_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

AC_TYPE_SIZE_T


dnl AC_HEADER
AC_HEADER_STDBOOL
AC_HEADER_TIME

dnl if we use headers that might not be on every platform, add them here
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(limits.h)
AC_CHECK_HEADERS(malloc.h)
AC_CHECK_HEADERS(stdarg.h)
AC_CHECK_HEADERS(syslog.h)
AC_CHECK_HEADER(sys/sysinfo.h,
       AC_DEFINE(HAVE_SYSINFO, 1, Define if sysinfo.h is present))
AC_CHECK_HEADERS(sys/time.h)
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STRFTIME
AC_CHECK_FUNCS(bzero)
AC_CHECK_FUNCS(gethostname)
AC_CHECK_FUNCS(gettimeofday)
AC_CHECK_FUNCS(index)
AC_CHECK_FUNCS(memset)
AC_CHECK_FUNCS(mkfifo)
AC_CHECK_FUNCS(socket)
AC_CHECK_FUNCS(sqrt)
AC_CHECK_FUNCS(stat)
AC_CHECK_FUNCS(strchr)
AC_CHECK_FUNCS(strdup)
AC_CHECK_FUNCS(strnlen)
AC_CHECK_FUNCS(strtoull)
AC_CHECK_FUNCS(strstr)

dnl CHECK_LIB
AC_CHECK_LIB([crypto], [SHA1], [],
             [AC_MSG_ERROR(-lcrypto required. Install OpenSSL.)])

dnl Check for utilities that we need during the build process
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_CC
dnl Do not need: AC_PROG_CXX (false alarm in autoscan)

AC_MSG_CHECKING([for required gcc])
if test "x$GCC" = "x"; then
	AC_MSG_ERROR(no)
fi

CFLAGS=$USR_CFLAGS

AC_PROG_CPP
AC_PROG_RANLIB
AC_CHECK_PROG(HAVE_FIND, find, yes, no)

AC_SUBST(BUILD_CC)
AC_SUBST(BUILD_CFLAGS)
AC_SUBST(BUILD_LDFLAGS)

dnl use MPE profiling library
AC_ARG_WITH(mpe,
[  --with-mpe=path         Use MPE profiling library installed in "path"],
    if test "x$withval" = "xyes" ; then
           AC_MSG_ERROR(--with-mpe must be given a pathname)
    else
        CFLAGS="$CFLAGS -I$withval/include"
        LDFLAGS="$LDFLAGS -L$withval/lib"
        LIBS="$LIBS -lmpe -lmpich"
        AC_MSG_CHECKING(for valid mpe install)
	AC_TRY_LINK(
              [#include <mpe.h>],
	      [
	      int eventID_begin, eventID_end;
	      eventID_begin = MPE_Log_get_event_number();
	      eventID_end   = MPE_Log_get_event_number();
	      MPE_Describe_state( eventID_begin, eventID_end, "Amult", "bluegreen" );
	      MPE_Log_event( eventID_begin, 0, (char *)0 );
	      MPE_Log_event( eventID_end, 0, (char *)0 );
	      ],
	      [
	      AC_DEFINE(HAVE_MPE, 1, [Use the MPE profiling library])
	      AC_MSG_RESULT(ok)],
	      AC_MSG_ERROR(Invalid path to mpe install))
    fi
)

dnl Stop using dmalloc - use valgrind
AC_ARG_WITH(dmalloc,
		[  --with-dmalloc           Use dmalloc],
   if test "x$withval" = "xyes" ; then
           AC_MSG_ERROR(--with-dmalloc must be given a pathname)
   else
        AC_CHECK_FILE(${withval}/include/dmalloc.h,
		        DMALLOC_LOCATION=${withval}
                        AC_MSG_RESULT([using dmalloc in ${withval}]),
		        AC_MSG_ERROR([Could not find dmalloc in ${withval}]))
   fi
)
AC_SUBST(DMALLOC_LOCATION)

dnl Optimized code, but never override CFLAGS from command line or environment.
AC_ARG_ENABLE(fast,
[  --enable-fast           Disable optional debugging, enable optimizations.],
[
if test "x$USR_CFLAGS_SET" = "xno"; then
    CFLAGS="$CFLAGS -DNDEBUG -O3 -DGOSSIP_DISABLE_DEBUG"
fi
])

# default CFLAGS is -g -O2, unless user set CFLAGS or asked for --enable-fast
if test "x$USR_CFLAGS_SET" = "xno" && test "x$enable_fast" != "xyes"; then
    CFLAGS="$CFLAGS -g -O2"
fi

dnl Disk types...

dnl Disk operation debugging
AC_ARG_WITH(disk-printf,
            AS_HELP_STRING(
                 [--with-disk-printf],
                 [Just print virtual disk operations]),
                 [AC_DEFINE([USE_DISK_PRINTF], [], [Use cmpi_disk_printf.c])
                  USE_DISK_PRINTF=1],
                 [USE_DISK_PRINTF=0])
AC_SUBST(USE_DISK_PRINTF)

dnl DISKSIM options
AC_ARG_WITH(disksim,
                 AS_HELP_STRING(
                 [--with-disksim],
                 [location of DISKSIM]),
                 [AC_CHECK_FILE(${withval}/src/disksim_interface.h,
                  [], [USE_DISKSIM=0])
                  if test "$USE_DISKSIM" = 0 ; then
                     AC_MSG_ERROR(Could not find DISKSIM in $withval)
                  fi
                  AC_MSG_RESULT([using disksim in $withval...])
                  AC_DEFINE_UNQUOTED([DISKSIM_LOCATION],
                                     "$withval", [DISKSIM Location])
                  USE_DISKSIM=$withval
                 ],
                 [
                  AC_MSG_RESULT([Not using DISKSIM...])
                  USE_DISKSIM=0
                 ])
AC_SUBST(USE_DISKSIM)

if test "$USE_DISK_PRINTF" = 0 -a "$USE_DISKSIM" = 0 ; then
   AC_MSG_RESULT([omitting disk operations...])
   USE_DISK_VOID=1
   AC_DEFINE_UNQUOTED([USE_DISK_VOID], [], [Use cmpi_disk_void.c])
   AC_SUBST(USE_DISK_VOID)
fi

USE_DISKSIM=
AC_SUBST(USE_DISKSIM)

STRICT_CFLAGS=
AC_ARG_ENABLE(strict,
[  --enable-strict         Turn on strict compiler warnings],
    STRICT_CFLAGS=1)
AC_SUBST(STRICT_CFLAGS)

dnl Check if user actually wants to see all our build output (compiles, links, etc.).
AC_ARG_ENABLE(verbose-build,
[  --enable-verbose-build  Enables full output during build process],
QUIET_COMPILE=0,
QUIET_COMPILE=1)

AC_SUBST(QUIET_COMPILE)

dnl Check if we are on a SLURM system
AC_CHECK_PROG(HAVE_SRUN, srun, yes, no)
AC_SUBST(HAVE_SRUN)

OPENSSL=""
AC_ARG_WITH(openssl,
    AS_HELP_STRING([--with-openssl], [location of OpenSSL]),
    if test x$withval = xyes; then
        AC_MSG_ERROR(--with-openssl must be given a pathname)
    else
        OPENSSL_LOCATION="${withval}"
	AC_MSG_RESULT([using OpenSSL in ${withval}])
    fi
)
AC_SUBST(OPENSSL_LOCATION)

dnl find mpicc

dnl check --with-mpi path first
AC_ARG_WITH(mpi,
    AS_HELP_STRING([--with-mpi], [location of MPI]),
    if test x$withval = xyes; then
        AC_MSG_ERROR(--with-mpi must be given a pathname)
    else
        MPICC="${withval}/bin/mpicc"
        GOT_MPICC="1"
        USE_MPI="${withval}"
    fi
)
dnl failing that, check path
if test x$GOT_MPICC != x1; then
    AC_CHECK_PROG(HAVE_MPICC, mpicc, yes, no)
    if test x$HAVE_MPICC != xyes; then
        AC_MSG_ERROR(cannot find mpicc; please use --with-mpi)
    else
        MPICC=mpicc
        GOT_MPICC="1"
        TMP_MPI_PATH=`which mpicc`
        dnl strip off /bin/mpicc to get "MPI" path
        TMP_MPI_PATH=`dirname $TMP_MPI_PATH`
        USE_MPI=`dirname $TMP_MPI_PATH`
    fi
fi
AC_SUBST(USE_MPI)

MPIEXEC=mpiexec
AC_ARG_WITH(mpiexec,
    AS_HELP_STRING([--with-mpiexec],
                   [name of mpiexec (e.g., mpiexec.hydra)]),
    MPIEXEC="${withval}"
)
AC_SUBST(MPIEXEC)

MPIARGS=
AC_ARG_WITH(mpi-args,
    AS_HELP_STRING([--with-mpi-args],
                   [arguments to mpiexec (e.g., -f hosts)]),
    MPIARGS="${withval}"
)
AC_SUBST(MPIARGS)

dnl does the mpi compiler that we found actually work?
saveCC="$CC"
CC="$MPICC"
AC_MSG_CHECKING(whether the mpicc compiler works)
AC_TRY_COMPILE([#include <mpi.h>], [int ret = MPI_Init(0, (void*)0)],
    AC_MSG_RESULT(yes),
    AC_MSG_RESULT(no)
    AC_MSG_ERROR($CC doesn't appear to be a valid MPI compiler)
)
CC=$saveCC

dnl DHT type selection for CMPI:
dnl Results are substituted in below:

AC_ARG_ENABLE([table-dense-1],
		 AS_HELP_STRING(
		  [--enable-table-dense-1],
		  [use dense-1 table implementation]),
		 [AC_MSG_RESULT( [using dense-1 table implementation...] )
		  AC_DEFINE([USE_TABLE_DENSE_1], [1], [Using DENSE-1])
 		  AC_DEFINE([USE_COMM_WORLD],    [1], [Using MPI_COMM_WORLD])
		  USE_TABLE_DENSE_1=1
                  USE_COMM_WORLD=1],
 		 [USE_TABLE_DENSE_1=0])

AC_ARG_ENABLE([table-kda-2A],
 		 AS_HELP_STRING(
 	 	  [--enable-table-kda-2A],
		  [use kademlia-2A implementation]),
 		 [AC_MSG_RESULT([using kademlia-2A implementation...])
		  AC_DEFINE([USE_TABLE_KDA_2A], [1], [Using Kademlia-2A])
 		  AC_DEFINE([USE_COMM_WORLD],   [1], [Using MPI_COMM_WORLD])
		  USE_TABLE_KDA_2A=1
                  USE_COMM_WORLD=1],
                 [USE_TABLE_KDA_2A=0])

AC_ARG_ENABLE([table-kda-2B],
 		 AS_HELP_STRING(
 	 	  [--enable-table-kda-2B],
		  [use kademlia-2B implementation]),
 		 [AC_MSG_RESULT([using kademlia-2B implementation...])
		  AC_DEFINE([USE_TABLE_KDA_2B], [1],
		            [Using Kademlia-2B])
 		  AC_DEFINE([USE_COMM_WORLD],   [0],
                            [Not using MPI_COMM_WORLD])
		  USE_TABLE_KDA_2B=1
		  USE_COMM_WORLD=0],
                 [USE_TABLE_KDA_2B=0])

dnl Table results:
AC_SUBST(USE_TABLE_DENSE_1)
AC_SUBST(USE_TABLE_KDA_2A)
AC_SUBST(USE_TABLE_KDA_2B)
AC_SUBST(USE_COMM_WORLD)

dnl C-MPI mode selection:
USE_CMPI_MODE=MONO
AC_ARG_ENABLE([mode-mono],
	 AS_HELP_STRING(
 	 	  [--enable-mode-mono],
		  [use mode-mono node/client topology]),
 		 [AC_MSG_RESULT([using mode-mono...])
                 USE_CMPI_MODE=MONO])

AC_ARG_ENABLE([mode-pairs],
	 AS_HELP_STRING(
 	 	  [--enable-mode-pairs],
		  [use mode-pairs node/client topology]),
 		 [AC_MSG_RESULT([using mode-mono...])
                 USE_CMPI_MODE=PAIRS])
AC_SUBST(USE_CMPI_MODE)

AC_ARG_ENABLE([driver],
		 AS_HELP_STRING(
	 	  [--enable-driver],
		  [use CMPI driver]),
 		 [AC_MSG_RESULT( [using CMPI driver...] )
		  AC_DEFINE([USE_DRIVER], [1], [Using CMPI driver])
		  USE_DRIVER=1],
                 [USE_DRIVER=0])
AC_SUBST(USE_DRIVER)

AC_ARG_ENABLE([cmpi-io],
 		 AS_HELP_STRING(
 	 	  [--enable-cmpi-io],
 		  [use CMPI-IO]),
  		 [AC_MSG_RESULT( [using CMPI-IO...] )
 		  AC_DEFINE([USE_CMPI_IO], [1], [Using CMPI-IO])
 		  USE_CMPI_IO=1],
                  [USE_CMPI_IO=0])
AC_SUBST(USE_CMPI_IO)

AC_ARG_ENABLE([tests],
		       AS_HELP_STRING(
			[--enable-tests],
			[enable test modules]),
		[AC_MSG_RESULT([enabling test modules...])
		 ENABLE_TESTS="yes"], [])
AC_SUBST(ENABLE_TESTS)

dnl Disable -rdynamic if we don't have it.
dnl Use it if we can for backtraces.
dnl From: https://www.phpcompiler.org/lists/phc-general/2006-December/000619.html
AC_DEFUN(
	AC_GCC_SUPPORTS_RDYNAMIC,
	[
	AC_PATH_PROG([grep], [grep])
	echo -n "checking if the compiler supports -rdynamic... "
	if test "X${ac_compiler_gnu}X" = "XyesX"; then
	   if test "X`${CC} -dumpspecs | ${grep} rdynamic`X" != "XX"; then
		echo "yes"
		AC_SUBST([RDYNAMIC], ["-rdynamic"])
	   else
		echo "no"
		AC_SUBST([RDYNAMIC], [])
	   fi
	else
	   echo "no"
	   AC_SUBST([RDYNAMIC], [])
	fi
	]
)
AC_GCC_SUPPORTS_RDYNAMIC([RDYNAMIC])


dnl Temporarily allow writes to configure-controlled files:
find . -name module.mk -exec chmod u+w "{}" \;
test -f Makefile && chmod u+w Makefile

dnl output final version of top level makefile and subdirectory
dnl    makefile includes
AC_CONFIG_FILES(
	Makefile
	src/gossip/module.mk
	src/adts/module.mk
	src/cmpi/module.mk
	src/cmpi-cp/module.mk
	src/cmpi-db/module.mk
	src/mpi_tools/module.mk
	src/mpirpc/module.mk
	src/dense-1/module.mk
	src/kda-2/module.mk
	)

if test "x$ENABLE_TESTS" = "xyes" ; then
  AC_CONFIG_FILES(
	test/adts/module.mk
  	test/cmpi/module.mk
  	test/cmpi-db/module.mk
  	test/driver/module.mk
  	test/gossip/module.mk
  	test/mpirpc/module.mk
  	test/mpi_tools/module.mk
  	)
  if test "x$USE_CMPI_IO" = "x1" ; then
     AC_CONFIG_FILES(
       	test/cmpi-io/module.mk
	)
  fi
fi

AC_OUTPUT

dnl Disallow writes to configure-controlled files:
find . -name module.mk -exec chmod u-w "{}" \;
chmod u-w Makefile

AC_MSG_RESULT()
AC_MSG_RESULT([CMPI version string: $CMPI_VERSION])

AC_MSG_RESULT()
