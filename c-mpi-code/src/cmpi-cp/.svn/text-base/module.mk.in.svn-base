
# CMPI-CP is not an MPI program

DIR := src/cmpi-cp

PROGS += bin/cmpi-cp
CMPICP_SRC += src/cmpi-cp/cmpi-cp.c

CMPICP_OBJS = src/cmpi-cp/cmpi-cp.o src/cmpi/accessor.o src/mpi_tools/io_tools.o

ifneq ($(DMALLOC_LIB),)
	CMPICP_LIBS += -L $(DMALLOC_LIB) -l dmalloc
endif

src/cmpi-cp/cmpi-cp.o: src/cmpi-cp/cmpi-cp.c
	$(Q) "  CC		$(@) "
	$(E) $(CC) -c $(CFLAGS) $(<) -o $(@)

bin/cmpi-cp: $(CMPICP_OBJS)
	$(Q) "  LINK		$(@) "
	$(E) install -d bin
	$(E) $(CC) $(CMPICP_OBJS) $(CMPICP_LIBS) -o $(@)
