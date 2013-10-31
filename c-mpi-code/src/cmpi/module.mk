
DIR := src/cmpi

CMPI_SRC :=
CMPI_SRC += $(DIR)/cmpi.c
CMPI_SRC += $(DIR)/node.c
CMPI_SRC += $(DIR)/driver.c
CMPI_SRC += $(DIR)/accessor.c

# Select CMPI mode:
ifeq (MONO,MONO)
	MODE := $(DIR)/mode_mono.c
endif
ifeq (MONO,PAIRS)
	MODE := $(DIR)/mode_pairs.c
endif

CMPI_SRC += $(MODE)

ifeq (@USE_TABLE_DENSE@,1)
	CMPI_SRC += $(DIR)/dense.c
	CMPI_SRC += $(DIR)/driver.c
endif

ifeq (0,1)
	CMPI_SRC += $(DIR)/driver.c
endif

ifeq (0,1)
	CMPI_SRC += $(DIR)/cmpi-io.c
endif

ifeq (1,0)
	CMPI_SRC += $(DIR)/client.c
endif

ifeq (0,1)
	CMPI_SRC += $(DIR)/cmpi_disk_printf.c
endif

ifeq (1,1)
	CMPI_SRC += $(DIR)/cmpi_disk_void.c
endif

CMPI_OBJS += $(patsubst %.c, %.o, $(CMPI_SRC))
CMPI_OBJS += $(MPIRPC_OBJS)

CMPI = lib/libcmpi.a

$(CMPI): $(CMPI_OBJS) $(TABLE_OBJS)
	$(Q) "  AR		$@"
	$(E) install -d lib
	$(E) ar rcs $(@) $(CMPI_OBJS) $(TABLE_OBJS)
