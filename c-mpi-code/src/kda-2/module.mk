
DIR := src/kda-2

ifeq (0,1)
	USE_TABLE_KDA = 1
endif

ifeq (0,1)
	USE_TABLE_KDA = 1
endif

ifeq ($(USE_TABLE_KDA),1)
	KDA_SRC += $(DIR)/kademlia.c
	KDA_SRC += $(DIR)/neighbor.c
	KDA_SRC += $(DIR)/cmpi_kademlia.c
endif

ifeq (0,1)
	KDA_SRC += $(DIR)/conn-A.c
endif

ifeq (0,1)
	KDA_SRC += $(DIR)/conn-B.c
endif

KDA_OBJS := $(patsubst %.c,%.o, $(KDA_SRC))
TABLE_OBJS += $(KDA_OBJS)
