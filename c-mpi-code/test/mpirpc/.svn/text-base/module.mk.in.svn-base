
DIR := test/mpirpc

TEST_MPIRPC_SRC    += $(shell find $(DIR) -name "*.c")
TEST_MPIRPC_OBJS    = $(patsubst %.c, %.o,   $(TEST_MPIRPC_SRC))
TEST_MPIRPC_OUTPUT  = $(patsubst %.c, %.out, $(TEST_MPIRPC_SRC))

TEST_SRC    += $(TEST_MPIRPC_SRC)
TEST_OBJS   += $(TEST_MPIRPC_OBJS)
TEST_PROGS  += $(patsubst %.c, %.x, $(TEST_MPIRPC_SRC))
TEST_OUTPUT += $(TEST_MPIRPC_OUTPUT)

$(DIR)/test%.x: $(DIR)/test%.o lib/libmpirpc.a
	$(Q) "  LINK		$(@) "
	$(E) $(MPICC) $(MPE) $(<) lib/libmpirpc.a $(LIBS) -o $(@)

$(DIR)/test%.out: $(DIR)/test%.x
	$(Q) "  TEST		$(@) "
#	$(E) $(MPDCHECK)
	$(E) $(patsubst %.x, %.zsh, $(<)) $(<) $(LAUNCH)

$(DIR)/test-success.out: test/adts/test-success.out \
                         $(TEST_MPIRPC_OUTPUT)
	$(Q) "  TOUCH		$(@) "
	$(E) touch $(@)
