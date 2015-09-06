CC = sdcc
LD = sdcc
#LD = arm-none-eabi-gcc
#AS = arm-none-eabi-as
AR = sdar
#GDB = arm-none-eabi-gdb
#OBJDUMP = arm-none-eabi-objdump

CFLAGS = -c
CFLAGS += -mstm8
CFLAGS += --std-c99
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

LDFLAGS = -mstm8
