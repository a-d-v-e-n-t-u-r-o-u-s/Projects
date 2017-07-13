CC = sdcc
LD = sdcc
AR = sdar
LOADER = stm8flash

LIBPREFIX := lib
LIBEXT := .lib

CFLAGS = -c
CFLAGS += -DSDCC
CFLAGS += -mstm8
CFLAGS += --std-c99
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

LDFLAGS = -mstm8

LOADER_FLAGS += -cstlink
LOADER_FLAGS += -s 0x8000
LOADER_FLAGS += -pstm8l150
LOADER_FLAGS += -w $(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).ihx
