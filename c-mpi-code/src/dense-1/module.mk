
DIR := src/dense-1

ifeq (1,1)
	DENSE_SRC += $(DIR)/dense.c
	DENSE_SRC += $(DIR)/cmpi_dense.c
endif

DENSE_OBJS := $(patsubst %.c, %.o, $(DENSE_SRC))
TABLE_OBJS += $(DENSE_OBJS)
