
DIR := src/mpi_tools

MPITOOLS_SRC :=
MPITOOLS_SRC += $(DIR)/mpi_tools.c
MPITOOLS_SRC += $(DIR)/io_tools.c

MPITOOLS_OBJS = $(patsubst %.c, %.o, $(MPITOOLS_SRC))
