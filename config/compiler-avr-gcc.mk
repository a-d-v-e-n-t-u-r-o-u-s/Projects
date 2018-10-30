CC = avr-gcc
LD = avr-gcc
AR = avr-ar
LOADER = avrdude

LIBPREFIX := lib
LIBEXT := .lib

#CDEFS =

CFLAGS = -c
CFLAGS += -Wall
CFLAGS += -mmcu=avr4
#CFLAGS += --std-c99
CFLAGS += $(addprefix -I,$(INCLUDE_DIR))
CFLAGS += $(addprefix -D,$(CDEFS))
CFLAGS += -o $(OBJECTS_DIR)/$(@F)

#LDFLAGS = -mmcpu=avr4

LOADER_FLAGS += -c usbasp
LOADER_FLAGS += -p m8
LOADER_FLAGS += -U flash:w:$(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).hex
