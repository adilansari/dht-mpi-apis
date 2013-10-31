
DIR := test/driver

TEST_DRIVER_SRC += $(DIR)/test-cmd-quit.c  \
                   $(DIR)/test-cmd-sleep.c \
                   $(DIR)/test-cmd-put.c   \
                   $(DIR)/test-cmd-get.c

TEST_DRIVER_OBJS   := $(patsubst %.c, %.o, $(TEST_DRIVER_SRC))
TEST_DRIVER_OUTPUT = test/driver/test-quit.out 	\
                     test/driver/test-sleep.out \
                     test/driver/test-put.out 	\
                     test/driver/test-get.out

TEST_HELPER_SRC := test/driver/test_helpers.c \
                   test/driver/test-cmd.c     \
                   test/driver/test_driver_fifo.c

TEST_DRIVER_OBJS += $(patsubst %.c, %.o, $(TEST_HELPER_SRC))

TEST_SRC    += $(TEST_DRIVER_SRC)
TEST_OBJS   += $(TEST_DRIVER_OBJS)

TEST_DRIVER_CMDS = $(shell find test/driver -name test-cmd-*.c )
TEST_PROGS  += $(patsubst %.c, %.x, $(TEST_DRIVER_CMDS))
TEST_PROGS  += test/driver/test_driver.x
TEST_OUTPUT += $(TEST_DRIVER_OUTPUT)

TEST_DRIVER = test/driver/test_driver.x
TEST_DRIVER_IMPL = test/driver/test_driver_fifo.o

ifeq (MONO,MONO)
	NODES := 5
	CLIENTS := 1
else
	NODES := 3
	CLIENTS := 3
endif

$(TEST_DRIVER): $(TEST_DRIVER_IMPL) test/driver/test_helpers.o $(CMPI)
	$(Q) "  LINK		$(@) "
	$(E) $(MPICC) $(MPE) $(<) test/driver/test_helpers.o \
                            $(CMPI) $(LIBS) -o $(@)

test/driver/test%.x: test/driver/test%.o test/driver/test_helpers.o \
                       test/driver/test-cmd.o $(CMPI)
	$(Q) "  LINK		$(@) "
	$(E) $(MPICC) $(MPE) $(<) test/driver/test_helpers.o \
                     test/driver/test-cmd.o $(CMPI) $(LIBS) -o $(@)

test/driver/test%.out: test/driver/test%.zsh $(TEST_PROGS) $(TEST_DRIVER)
	$(Q) "  TEST		$(@) "
	$(E) LAUNCH=$(LAUNCH) test/driver/test.zsh $(@) \
                    $(NODES) $(CLIENTS) $(<) $(DRIVER_DIR)

test/driver/test-success.out: test/cmpi/test-success.out \
                              $(TEST_DRIVER_OUTPUT)
