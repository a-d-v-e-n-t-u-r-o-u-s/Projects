CC = sdcc
LD = sdcc
AR = sdar

CFLAGS = -c
CFLAGS += -mstm8
CFLAGS += --std-c99
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

LDFLAGS = -mstm8
