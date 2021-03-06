
TEST_CMPIDB_OUTPUT = test/cmpi-db/test-quit.out \
                     test/cmpi-db/test-cp1.out \
                     test/cmpi-db/test-loop.out

TEST_OUTPUT += $(TEST_CMPIDB_OUTPUT)

CMPIDB = bin/cmpi-db
CMPICP = bin/cmpi-cp
CMPIDBQUIT = bin/cmpi-db-quit
CMPIDB_TOOLS = $(CMPIDB) $(CMPICP) $(CMPIDBQUIT)

ifeq (MONO,MONO)
	NODES := 5
	CLIENTS := 1
else
	NODES := 3
	CLIENTS := 3
endif

test/cmpi-db/test-%.out: test/cmpi-db/test-%.zsh $(CMPIDB_TOOLS)
	$(Q) "  TEST		$(@) "
#	$(E) $(MPDCHECK)
	$(E) $(<) $(@) $(NODES) $(CLIENTS)

test/cmpi-db/test-success.out: test/driver/test-success.out \
                               $(TEST_CMPIDB_OUTPUT)
	$(Q) "  TOUCH		$(@) "
	$(E) touch $(@)
