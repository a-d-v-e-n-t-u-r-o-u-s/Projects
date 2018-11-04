CC = avr-gcc
LD = avr-gcc
AR = avr-ar
LOADER = avrdude

LIBPREFIX := lib
LIBEXT := .a

CDEFS = F_CPU=8000000UL

ARFLAGS = rcs

CFLAGS = -c
CFLAGS += -Wall
CFLAGS += -mmcu=atmega8
CFLAGS += -Os
#CFLAGS += --std-c99
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += $(addprefix -D,$(CDEFS))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

LDFLAGS += -mmcu=atmega8
LDFLAGS += -Wl,-Map,$(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).map
LDFLAGS += -Wl,--cref
LDFLAGS += -Wl,-L$(LIB_DIR)

LOADER_FLAGS += -c usbasp
LOADER_FLAGS += -p m8
LOADER_FLAGS += -U flash:w:$(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).elf
