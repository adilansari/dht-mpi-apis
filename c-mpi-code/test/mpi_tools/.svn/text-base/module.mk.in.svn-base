
DIR := test/mpi_tools

TEST_MPITOOLS_SRC    = $(shell find $(DIR) -name "*.c")
TEST_MPITOOLS_OBJS   = $(patsubst %.c, %.o,   $(TEST_MPITOOLS_SRC))
TEST_MPITOOLS_OUTPUT = $(patsubst %.c, %.out, $(TEST_MPITOOLS_SRC))

TEST_SRC    += $(TEST_MPITOOLS_SRC)
TEST_OBJS   += $(TEST_MPITOOLS_OBJS)
TEST_OUTPUT += $(TEST_MPITOOLS_OUTPUT)
TEST_PROGS  += $(patsubst %.c, %.x,   $(TEST_MPITOOLS_SRC))

$(DIR)/test%.out: test/mpi_tools/test%.x
	$(Q) "  TEST		$(@) "
	$(E) $(LAUNCH) $(<) > $(@) 2>&1

$(DIR)/test%.x: test/mpi_tools/test%.o $(MPITOOLS_OBJS)
	$(Q) "  LINK		$(@) "
	$(E)$(MPICC) $(MPE) $(<) $(MPITOOLS_OBJS) $(LIBS) -o $(@)

$(DIR)/test-success.out: $(TEST_MPITOOLS_OUTPUT)
	$(Q) "  TOUCH		$(@) "
	$(E) touch $(@)
