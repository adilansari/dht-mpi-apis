
DIR := src/cmpi-db

PROGS  += bin/cmpi-db
PROGS  += bin/cmpi-db-quit
CMPIDB_SRC  = src/cmpi-db/cmpi-db-fifo.c \
              src/cmpi-db/cmpi-db-fifo-quit.c
CMPIDB_OBJS = $(patsubst %.c, %.o, $(CMPIDB_SRC))

CMPIDB = bin/cmpi-db

$(CMPIDB): src/cmpi-db/cmpi-db-fifo.o $(CMPI)
	$(Q) "  LINK		$(@)"
	$(E) install -d bin
	$(E) $(MPICC) $(MPE) src/cmpi/node.o src/cmpi/driver.o \
                     $(<) $(CMPI) $(LIBS) -o $(@)

bin/cmpi-db-quit: src/cmpi-db/cmpi-db-fifo-quit.o $(CMPI) bin
	$(Q) "  LINK		$(@)"
	$(E) $(MPICC) $(<) $(CMPI) -o $(@)
