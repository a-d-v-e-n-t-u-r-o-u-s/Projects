CC = gcc
LD = gcc
AR = ar

CDEFS = STM8L15X_MD

CFLAGS = -c
CFLAGS += --std=c99
CFLAGS += -Wall
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += $(addprefix -D,$(CDEFS))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

#LDFLAGS = -mstm8
