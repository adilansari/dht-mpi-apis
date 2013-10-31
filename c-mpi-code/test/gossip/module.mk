
DIR := test/gossip

TEST_GOSSIP_SRC  += $(shell find $(DIR) -name "*.c")
TEST_GOSSIP_OBJS += $(patsubst %.c, %.o,   $(TEST_GOSSIP_SRC))
TEST_PROGS       += $(patsubst %.c, %.x,   $(TEST_GOSSIP_SRC))
TEST_GOSSIP_OUTPUT = $(patsubst %.c, %.out, $(TEST_GOSSIP_SRC))
TEST_OBJS += $(TEST_GOSSIP_OBJS)
TEST_OUTPUT += $(TEST_GOSSIP_OUTPUT)

$(DIR)/test%.x: $(DIR)/test%.o $(GOSSIP_OBJS)
	$(Q) "  LINK		$(@) "
	$(E) $(MPICC) $(<) $(GOSSIP_OBJS) $(LIBS) -o $(@)

$(DIR)/test%.out: $(DIR)/test%.x
	$(Q) "  TEST		$(@) "
	$(E) LAUNCH=$(LAUNCH) $(patsubst %.x, %.zsh, $(<)) $(<)

$(DIR)/test-success.out: test/mpi_tools/test-success.out \
                         $(TEST_GOSSIP_OUTPUT)
	$(Q) "  TOUCH		$(@) "
	$(E) touch $(@)
