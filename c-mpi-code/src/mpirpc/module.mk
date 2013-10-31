
DIR := src/mpirpc

MPIRPC_SRC := $(DIR)/mpirpc.c

MPIRPC_OBJS := $(patsubst %.c, %.o, $(MPIRPC_SRC))
MPIRPC_OBJS += $(ADTS_OBJS)

MPIRPC = lib/libmpirpc.a

lib/libmpirpc.a: $(MPIRPC_OBJS)
	$(Q) "  AR		$@"
	$(E) install -d lib
	$(E) ar crs $(@) $(MPIRPC_OBJS)
