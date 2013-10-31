GOSSIP_ENABLE_BACKTRACE =
# TODO: need to figure out how to tie in -rdynamic configure tests here, in top
# level makefile it is only enabled with $DEBUG regardless of configure
# outcome

DIR := src/gossip
GOSSIP_SRC := $(DIR)/gossip.c

ifdef GOSSIP_ENABLE_BACKTRACE
MODCFLAGS_$(DIR)/gossip.c := -DGOSSIP_ENABLE_BACKTRACE
endif

GOSSIP_OBJS = $(patsubst %.c, %.o, $(GOSSIP_SRC))
GOSSIP_OBJS += $(MPITOOLS_OBJS)
