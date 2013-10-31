
DIR := test/adts

TEST_ADTS_SRC    += $(shell find $(DIR) -name "*.c")
TEST_ADTS_OBJS    = $(patsubst %.c, %.o,   $(TEST_ADTS_SRC))
TEST_ADTS_OUTPUT  = $(patsubst %.c, %.out, $(TEST_ADTS_SRC))

TEST_SRC    += $(TEST_ADTS_SRC)
TEST_OBJS   += $(TEST_ADTS_OBJS)
TEST_OUTPUT += $(TEST_ADTS_OUTPUT)
TEST_PROGS  += $(patsubst %.c, %.x, $(TEST_ADTS_SRC))

$(DIR)/test%.x: $(DIR)/test%.o $(ADTS_OBJS)
	$(Q) "  LINK		$(@) "
	$(E) $(MPICC) $(MPE) $(<) $(ADTS_OBJS) $(LIBS) -o $(@)

$(DIR)/test%.out: $(DIR)/test%.x
	$(Q) "  TEST		$(@) "
	$(E) LAUNCH=$(LAUNCH) test/adts/runtest.zsh $(<) $(@) 2>&1

$(DIR)/test-success.out: test/gossip/test-success.out \
                         $(TEST_ADTS_OUTPUT)
	$(Q) "  TOUCH		$(@) "
	$(E) touch $(@)
