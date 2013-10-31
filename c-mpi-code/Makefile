

# LIST OF TARGETS WHICH MAY BE USED WHEN RUNNING MAKE:
#
# all            default rule; builds libs, server, and test programs
# clean          cleans up make-created files
# cleaner        cleans up configure-created files
# distclean      _really_ cleans up; returns to pristine tree
# tags           generates etags file for use by editors
# test_results   run all tests

# INPUT CONTROL VARIABLES
# D : if 1, turn on debugging output in CMPI programs
# G : if 1, certain tests wait for GDB to attach
# S : if 0, turn off debugging symbols
# T : if 1, turn on MPE printf debugging
# V : if 1, turn on normal make output

# NOTABLE CONFIGURE OPTIONS
# 1) DMALLOC
# 2) DISKSIM

# GENERAL DOCUMENTATION
#
# This is a single makefile that runs the entire build process.
# There are no makefiles in subdirectories.
# For a general introduction to this approach, see:
# http://www.tip.net.au/~millerp/rmch/recu-make-cons-harm.html
#
# Each subdirectory contains a module.mk file that gets included
# when make is executed.  These module.mk files tell make about the
# files in each subdirectory that must be built, including any
# special case rules.  Make uses this information to generate a
# single dependency graph and orchestrate the build process from this
# top level directory.

# Generic makefile setup

# define a few generic variables that we need to use
DESTDIR =
srcdir = .
prefix = $(DESTDIR)/usr/local
datarootdir = $(DESTDIR)${prefix}/share
mandir = $(DESTDIR)${datarootdir}/man
exec_prefix = $(DESTDIR)${prefix}

SHELL = /bin/bash
INSTALL = /usr/bin/install -c
GNUC = @GNUC@

# OpenSSL
ifneq (,)
	OPENSSL_INCLUDE = /include
	OPENSSL_LIB     = /lib
endif

ENABLE_TESTS = yes

# MPI location
MPI     = /usr
MPICC   = /usr/bin/mpicc
MPIEXEC = /usr/bin/mpiexec
MPIEXEC = /usr/bin/mpiexec
# MPIARGS = -recvtimeout 10
MPIARGS = 

# Job launch selection (mpiexec or srun)
ifeq (no,no)
	LAUNCH = $(MPIEXEC) $(MPIARGS)
else
	LAUNCH = srun
endif

# DISKSIM location
DISKSIM=

# configure default is silent, unless --enable-verbose-build in
# which case QUIET_COMPILE will _not_ be defined.  Further allow
# silence to be overriden with "make V=1".
QUIET_COMPILE = 1
ifeq ($(D),1)
	DEBUG = 1
else
	DEBUG = 0
endif
ifeq ($(G),1)
	GDB_WAIT="-D GDB_WAIT"
endif
ifeq ($(S),0)
	GGDB = 0
else
	GGDB = 1
endif
ifeq ($(T),1)
	MPE =-mpe=mpitrace
endif
ifeq ($(V),1)
	QUIET_COMPILE = 0
endif

TEST_OPTIONS :=
ifeq ($(TEST01),1)
	TEST_OPTIONS += -D ENABLE_TEST01
endif

ifeq ($(QUIET_COMPILE),1)
  # Prints a short description of the action, does not show command
  Q=@echo
  E=@
else
  # Just echo the entire command
  Q=@echo >/dev/null
  E=
endif

# build which client libs
build_shared = @build_shared@
build_static = @build_static@

# Eliminate all default suffixes.  We want explicit control.
.SUFFIXES:

# PHONEY targets are targets that do not result in the generation
#    of a file that has the same name as the target.  Listing them
#    here keeps make from accidentally doing too much work (see GNU
#    make manual).
.PHONY: all clean dist distclean docs docsclean cscope tags codecheck cmpi mpirpc null

################################################################
# Find project subdirectories

# MODULES is a list of subdirectories that we wish to operate on.
#    They are identified by the presence of module.mk files (makefile
#    includes).
DIR :=
MODULES :=

# $(shell find . "(" -name gsl -prune -or -name "*.mk" ")" -a -type f )

# List of directories to search for headers.
INCLUDES := include

#################################################################
# Setup global flags

# These should all be self explanatory; they are standard flags
# for compiling and linking unless otherwise noted
CC = gcc
LD = gcc
BUILD_CC = 
BUILD_LD = 
BUILD_CFLAGS = 
BUILD_LDFLAGS = 
# make sure the srcdir include gets included first
CFLAGS = -std=gnu99 -Wall #  -g -O2  # -I $(srcdir)/include
LDFLAGS = -L@BUILD_ABSOLUTE_TOP@/lib
ifneq ($(DISKSIM)/src,)
	LDFLAGS += -L$(DISKSIM)/src
endif
LDFLAGS += 
LDSHARED = $(CC) -shared
PICFLAGS = -fPIC
LIBS += -lcrypto 

IFLAGS :=

ifneq ($(DISKSIM),)
	LIBS += -L$(DISKSIM)/src -L$(DISKSIM)/libparam
	LIBS += -L$(DISKSIM)/libddbg -L$(DISKSIM)/diskmodel
	LIBS += -L$(DISKSIM)/memsmodel -ldisksim -lparam -lddbg
	LIBS +=	-ldiskmodel -lmems_internals -lmemsmodel
endif

ifneq ($(OPENSSL_LIB),)
	LIBS += -L $(OPENSSL_LIB)
endif
ifneq ($(DMALLOC_LIB),)
	LIBS += -L $(DMALLOC_LIB) -l dmalloc
endif

# Used by src/mpi_tools/mpi_tools.c:isqrt()
LIBS += -lm

STRICT_CFLAGS = 

ifeq ($(DEBUG),1)
	CFLAGS += -rdynamic -Wno-unused-variable
else
	CFLAGS += -O2
endif

ifeq ($(GGDB),1)
	CFLAGS += -ggdb3
endif

# turn on large file support by default
CFLAGS += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
IFLAGS += -I $(srcdir)
# add selected include directories
IFLAGS += $(patsubst %,-I $(srcdir)/%,$(INCLUDES))
ifneq ($(DISKSIM),)
	IFLAGS += -I $(DISKSIM) -I $(DISKSIM)/src
endif
ifneq ($(OPENSSL_INCLUDE),)
	IFLAGS += -I $(OPENSSL_INCLUDE)
endif
ifneq ($(DMALLOC_INCLUDE),)
	IFLAGS += -I $(DMALLOC_INCLUDE)
endif

CFLAGS += $(IFLAGS)

# optionally define dmalloc symbols
ifneq (,)
	CFLAGS += -D DMALLOC -D DMALLOC_FUNC_CHECK
endif

# add flags to enable MPI features in gossip
CFLAGS += -D __GOSSIP_ENABLE_MPI__

# add package version information
CFLAGS += -D CMPI_VERSION="\"0.1.1pre1-2013-10-31-180214\""
# optionally turn on debugging output
ifeq ($(DEBUG),1)
	CFLAGS += -D ENABLE_DEBUG
endif
ifeq ($(DEBUG_MALLOC),1)
	CFLAGS += -D ENABLE_DEBUG_MALLOC
endif

CFLAGS += $(GDB_WAIT)

# enabled packages
ifeq (0,1)
	CMPI_IO_P = $(CMPI_IO)
endif

# always want these gcc flags
GCC_CFLAGS := -pipe

ifneq (,$(STRICT_CFLAGS))
    GCC_CFLAGS += -Wcast-align -Wbad-function-cast
    GCC_CFLAGS += -Wmissing-prototypes -Wmissing-declarations
    GCC_CFLAGS += -Wundef -Wpointer-arith
    GCC_CFLAGS += -Wnested-externs
    GCC_CFLAGS += -Wredundant-decls
endif

################################################################
# Setup component specific flags

# you can optionally disable thread safety support in the client
# though it's not recommended unless *required*.
#
# run ./configure --help for information on how to do this cleanly.
LIBCFLAGS = # @LIBCFLAGS@

# LIBSRC is source code for libcmpi
LIBSRC :=
# Source code for Kademlia
KDA_SRC    :=
# Source code for Dense implementation
DENSE_SRC  :=
# Source code for ADTs
ADTS_SRC    :=
# Source code for DiskSim interface
DISKSIM_SRC :=

################################################################
# Default target forward pointer, to avoid other targets in make stubs
all:: cmpi

debug_variables:
	$(Q) "  DEBUGGING VARIABLES "
	$(Q) " "
	$(E)echo MPICC:   $(MPICC)
#	$(E)echo MODULES: $(MODULES)
#	$(E)echo DISKSIM: $(DISKSIM)
	$(E)echo CFLAGS:  $(CFLAGS)
	$(E)echo IFLAGS:  $(IFLAGS)
	$(E)echo LIBS:    $(LIBS)
	$(E)echo CMPI_DEPS: $(CMPI_DEPS)
	$(E)echo OPENSSL: $(OPENSSL_LOCATION)
	$(E)echo GOSSIP_OBJS:   $(GOSSIP_OBJS)
	$(E)echo ADTS_OBJS:     $(ADTS_OBJS)
	$(E)echo MPITOOLS_OBJS: $(MPITOOLS_OBJS)
	$(E)echo MPIRPC_OBJS:   $(MPIRPC_OBJS)
#	$(E)echo USE_TABLE_KDA_2A:  0
#	$(E)echo USE_TABLE_KDA_2B:  0
	$(E)echo USE_TABLE_DENSE_1: 1
	$(E)echo TABLE_OBJS:         $(TABLE_OBJS)
	$(E)echo CMPI_OBJS:          $(CMPI_OBJS)
#	$(E)echo TEST_CMPI_SRC:    $(TEST_CMPI_SRC)
#	$(E)echo TEST_DISKSIM_SRC: $(TEST_DISKSIM_SRC)
#	$(E)echo TEST_MPIRPC_SRC:  $(TEST_MPIRPC_SRC)
	$(E)echo TEST_SRC:         $(TEST_SRC)
	$(E)echo TEST_OBJS:        $(TEST_OBJS)
	$(E)echo TEST_HELPER_SRC:  $(TEST_HELPER_SRC)
	$(E)echo TEST_PROGS:       $(TEST_PROGS)
	$(E)echo TEST_DEPS:        $(TEST_DEPS)
	$(Q) " "

# Shortcuts...
CMPI_IO := lib/libcmpi-io.so

################################################################
# Makefile includes
# this is how we pull build information from all of the project
#    subdirectories, make sure to catch top level module.mk as well
DIR :=
# include $(MODULES)

PROGS :=
TABLE_OBJS :=
include src/mpi_tools/module.mk
include src/gossip/module.mk
include src/adts/module.mk
include src/mpirpc/module.mk
include src/kda-2/module.mk
include src/dense-1/module.mk
include src/cmpi/module.mk
include src/cmpi-db/module.mk
include src/cmpi-cp/module.mk

################################################################
# Test cases
# Test rules are heavily based on the definitions above

TEST_SRC :=
TEST_OBJS :=
TEST_PROGS :=
TEST_OUTPUT :=

include test/mpi_tools/module.mk
include test/gossip/module.mk
include test/adts/module.mk
include test/mpirpc/module.mk
include test/cmpi/module.mk
include test/driver/module.mk
include test/cmpi-db/module.mk
#include test/cmpi-io/module.mk

#TEST_HELPER_OBJS += $(patsubst  %.c, %.o, $(TEST_HELPER_SRC))
TEST_DEPS   += $(patsubst %.c,  %.d,   $(TEST_SRC))
TEST_DEPS   += $(patsubst %.c,  %.d,   $(TEST_HELPER_SRC))

################################################################
# Derived file lists

# NOTES: At this point, the subdirectory makefile includes have informed
#    us what the source files are.  Now we want to generate some
#    other lists (such as objects, executables, and dependency files)
#    by manipulating the lists of source files

# LIBOBJS is a list of objects to put in the client lib
#LIBOBJS     := $(patsubst %.c,%.o, $(filter %.c,$(LIBSRC)))

#DISKSIM_OBJS := $(patsubst %.c,%.o, $(filter %.c,$(DISKSIM_SRC)))

CMPI_OBJS := $(TABLE_OBJS) $(MPIRPC_OBJS) $(CMPI_OBJS)
# $(DISKSIM_OBJS)

MPIRPC_DEPS := $(patsubst %.o,%.d,$(MPIRPC_OBJS))
CMPI_DEPS   := $(patsubst %.o,%.d,$(CMPI_OBJS))
ifeq ($(ENABLE_TESTS),yes)
	CMPI_DEPS += $(TEST_DEPS)
endif

deps: $(CMPI_DEPS)

####################################################################
# Rules and dependencies

# default rule builds server, library, and applications
# all:: # $(SERVER) $(LIBRARIES)

mpirpc: $(MPIRPC)
cmpi:   $(CMPI)
tools:  bin/cmpi-db bin/cmpi-db-quit bin/cmpi-cp

#cmpi-io: $(CMPI_IO)

# Just like dir, but strip the slash off the end, to be pretty.
dirname = $(patsubst %/,%,$(dir $(1)))

# Generate the canonical in-tree location of a file, given a possibly
# out-of-tree reference.
# canonname = $(patsubst $(srcdir)/%,%,$(call dirname,$(1)))

# CMPI-IO must be enabled in configure --enable-cmpi-io
#$(CMPI_IO): $(CMPI) src/cmpi/cmpi-io.po
#	$(Q) "  MPICC  [PIC]    $@"
#	$(E)$(INSTALL) -d lib
#	$(E)$(MPICC) -shared $< $(CMPI) $(LIBS) -o $@

## Tools...

driver: src/cmpi/driver.o $(CMPI)
	$(Q) "  MPICC           $(@)"
	$(E)$(MPICC) src/cmpi/driver.o $(CMPI) $(LIBS) -o $(@)

ifeq (1,0)

# node executable is only built in the stand-alone node cases
#  (e.g., KDA-2B)
node: src/cmpi/node.o $(CMPI)
	$(Q) "  MPICC		$(@)"
	$(E)$(MPICC) $(MPE) src/cmpi/node.o $(CMPI) $(LIBS) -o $(@)

endif

unpublish: src/mpirpc/unpublish.o $(CMPI)
	$(Q) "  MPICC           $(@)"
	$(E) $(MPICC) $(<) $(CMPI) $(LIBS) -o $(@)

bin:
	$(Q) "  MKDIR		$(@)"
	$(E) mkdir -p $(@)

## Development tools...

bin/hex: src/kda-2/hex.o bin
	gcc $(<) -o $(@)

bin/unhex: src/kda-2/unhex.o bin
	gcc $(<) -o $(@)

bin/hexord: src/kda-2/hexord.o bin
	gcc $(<) -o $(@)

## Tests...

test_results: test/cmpi-db/test-success.out

tests: $(TEST_PROGS)

# Obsolete target: delete soon.
#cmpi-io-test: $(CMPI_IO) src/cmpi/cmpi-io-test.o
#	$(Q) "  MPICC           cmpi-io-test"
#	$(E)$(MPICC) src/cmpi/cmpi-io-test.o $(CMPI) $(LIBS) -o cmpi-io-test

# override default rule for building objects
%.o: %.c
	$(Q) "  MPICC		$@"
	$(E)$(MPICC) $(LIBCFLAGS) $(CFLAGS) $(TEST_OPTIONS) \
        $(call modcflags,$(<)) $(<) -c -o $(@)

.SECONDARY : %.o %.x

# handy rule to generate cpp-output file, for debugging
.PHONY: FORCE
%.i: %.c FORCE
	$(Q) "  CPP		$@"
	$(E)$(CC) $(LIBCFLAGS) $(CFLAGS) $(call modcflags,$<) $< -E -o $@

tags: TAGS

# Generate TAGS based on only files used by CMPI as configured:
TAGS: deps.txt
	$(Q) "  TAGS "
	$(E) etags $(ETAGS_FLAGS) `cat deps.txt`

# Extract all files from *.d files, format and sort:
# Formats to one file per line & extracts junk and extra files in *.d
deps.txt: $(CMPI_DEPS)
	$(Q) "  DEPS.TXT "
	$(E) cat $(CMPI_DEPS) | \
		fmt -w 1 | cut -f 1  | \
                sed 's/ //g;s/\\//g' | \
	        grep -v '.*\.o\>\|.*\.po\>\|.*\.d\>' | \
	        sort -u > $(@)

RM_ALL = find . -name .svn -prune -o -name $(1) -exec rm -fv \{\} \;

# top rule for cleaning up tree
clean:
	$(Q) "  CLEAN"
	$(E)rm -fv $(CMPI_DEPS) $(TEST_DEPS) \
                $(MPIRPC) $(CMPI) \
		$(ADTS_OBJS) $(MPIRPC_OBJS) $(CMPI_OBJS) \
                $(KDA_OBJS) $(CMPI_IO) \
                hex unhex hexord unpublish node driver        \
                $(TEST_OUTPUT) $(TEST_PROGS) $(TEST_OBJS)     \
                $(PROGS) $(CMPICP_OBJS) $(CMPIDB_OBJS) splint.out
	$(E) $(call RM_ALL, "*.failed"         )
	$(E) $(call RM_ALL, "*.bombed"         )
	$(E) $(call RM_ALL, "tool.out"         )
	$(E) $(call RM_ALL, "input.data"       )
	$(E) $(call RM_ALL, "output.data"      )
	$(E) $(call RM_ALL, "test-success.out" )
	$(E) $(call RM_ALL, "dmalloc*.out"     )
#	$(E)find . -name "*.avg"       -exec rm -fv \{\} \;
#	$(E)find . -name "*.per"       -exec rm -fv \{\} \;

# Allow quick (cached) reconfiguration: regeneration of Makefile
# some stuff that is cleaned in both distclean and dist targets
cleaner: clean
	$(Q) "  CLEANER"
	$(E)rm -fvr TAGS deps.txt \
	            maint/mpi-depend.sh         \
                    examples/fs.conf            \
	            autom4te*.cache             \
	            config.h.in~                \
                    $(srcdir)/cscope.out        \
                    $(srcdir)/cscope.files      \
	            aclocal.m4	                \
	            config.h.in                 \
                    autoscan.log                \
                    configure.scan              \
		    Makefile
	$(E)find . -name "*.i"       -exec rm -fv \{\} \;
	$(E)find . -name "*.d"       -exec rm -fv \{\} \;
	$(E)find . -name "module.mk" -exec rm -fv \{\} \;

# Allow regeneration of configure script
# _really_ clean the tree; should go back to pristine state
# except, don't remove generated .c files if this is a distributed release
distclean: cleaner docsclean
	$(Q) "  DISTCLEAN"
	$(E)rm -fv configure config.h cmpi-config.h*
	$(E)find . -name "*.dat"       -exec rm -fv \{\} \;

# this is where we include all of our automatic dependencies.
# NOTE: we wrap this in ifneq's in order to prevent the
#    dependencies from being generated for special targets that don't
#    require them
ifeq (,$(filter clean distclean dist docs cscope tags nodep deps,$(MAKECMDGOALS)))
  -include $(CMPI_DEPS)
endif

# add this as a make goal to disable rebuilding dependencies
.PHONY: nodep clean
.SECONDARY: $(TEST_OBJS) $(TEST_PROGS)
.PRECIOUS: $(TEST_OUTPUT) %.o
nodep:; @:

# default rule for generating dependency files (deps)
%.d: %.c
	$(Q) "  DEP		$@"
	$(E) CC="$(MPICC)" $(srcdir)/depend.sh \
             $(call dirname, $*) $(CFLAGS) $< > $@

SPLINTOPTS = +posixlib +gnuextensions #  -preproc

splint: $(CMPI_SRC)
	$(Q) "  SPLINT		splint.out"
	$(E) splint $(SPLINTOPTS) $(IFLAGS) -I $(MPI)/include $(CMPI_SRC) > splint.out

null:
