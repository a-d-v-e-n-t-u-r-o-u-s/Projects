CC = sdcc
LD = sdcc
AR = sdar
LOADER = stm8flash

LIBPREFIX := lib
LIBEXT := .lib

CDEFS = STM8L15X_MD

CFLAGS = -c
CFLAGS += -DSDCC
CFLAGS += -mstm8
CFLAGS += --std-c99
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += $(addprefix -D,$(CDEFS))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

LDFLAGS = -mstm8

LOADER_FLAGS += -c stlink
LOADER_FLAGS += -s 0x8000
LOADER_FLAGS += -p stm8l152?6
LOADER_FLAGS += -w $(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).ihx
