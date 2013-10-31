
DIR := test/cmpi

TEST_CMPI_SRC += $(shell find test/cmpi -name "*.c" ! -name test_helpers.c)
TEST_CMPI_OBJS    = $(patsubst %.c, %.o,   $(TEST_CMPI_SRC))
TEST_CMPI_OUTPUT  = test/cmpi/test-startup.out  \
                    test/cmpi/test-tables.out   \
		    test/cmpi/test-putget.out   \
                    test/cmpi/test-manyputs.out \
                    test/cmpi/test-update01.out \
                    test/cmpi/test-update02.out

TEST_HELPER_SRC := test/cmpi/test_helpers.c
TEST_CMPI_OBJS  += $(patsubst %.c, %.o,   $(TEST_HELPER_SRC))
TEST_CMPI_PROGS := $(patsubst %.c, %.x, $(TEST_CMPI_SRC))

TEST_SRC    += $(TEST_CMPI_SRC)
TEST_OBJS   += $(TEST_CMPI_OBJS)
TEST_PROGS  += $(TEST_CMPI_PROGS)
TEST_OUTPUT += $(TEST_CMPI_OUTPUT)


ifeq (MONO,MONO)
  NODES := 5
  CLIENTS := 1
else
  NODES := 3
  CLIENTS := 3
endif

cmpi_tests: $(TEST_CMPI_PROGS)

ifeq (1,1)

$(DIR)/test%.x: $(DIR)/test%.o $(DIR)/test_helpers.o src/cmpi/node.o $(CMPI)
	$(Q) "  LINK		$(@) "
	$(E) $(MPICC) $(MPE) $(<) test/cmpi/test_helpers.o src/cmpi/node.o \
                            $(CMPI) $(LIBS) -o $(@)


else

$(DIR)/test%.x: $(DIR)/test%.o $(DIR)/test_helpers.o src/cmpi/client.o $(CMPI)
	$(Q) "  LINK           $(@) "
	$(E)$(MPICC) $(MPE) $(<) $(DIR)/test_helpers.o src/cmpi/client.o $(CMPI) $(LIBS) -o $(@)

endif

$(DIR)/test%.out: $(DIR)/test%.x $(DIR)/test%.zsh
# $(E) $(MPDCHECK)
	$(Q) "  TEST		$(@) "
	$(E) $(patsubst %.x, %.zsh, $(<)) $(<) $(NODES) $(CLIENTS) $(LAUNCH)

$(DIR)/test-success.out: test/mpirpc/test-success.out \
                         $(TEST_CMPI_OUTPUT)
	$(Q) "  TOUCH		$(@) "
	$(E) touch $(@)
