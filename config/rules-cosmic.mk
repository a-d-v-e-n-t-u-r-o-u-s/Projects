include compiler-$(TARGET)-$(COMPILER).mk

CURDIR_RELATIVE := $(subst $(PROJECT_DIR),,$(CURDIR))
OBJECTS_DIR := $(BUILD_DIR)$(CURDIR_RELATIVE)
OBJECTS_DIR := $(subst /,$(DELIM),$(OBJECTS_DIR))

OBJECTS := $(addsuffix .o,$(basename $(SOURCE)))

INCLUDE_DIR += ../../projects/$(PROJECT) $(patsubst %,../../%,$(GLOBAL_INCLUDE_DIR))

vpath
vpath %.c 		$(subst  ,:,$(SOURCE_DIR))
vpath %.s 		$(subst  ,:,$(SOURCE_DIR))
vpath %.o 		$(OBJECTS_DIR)

.SUFFIXES:
.SUFFIXES: .c .o .s

.PHONY: all

LIBRARY := $(if $(LIBRARY),$(LIBPREFIX)$(LIBRARY)$(LIBEXT))

all: $(OBJECTS_DIR) $(LIBRARY) $(EXECUTABLE)

$(OBJECTS_DIR):
	@echo Creating object directory ...
	$(MKDIR) $(OBJECTS_DIR)

$(OBJECTS): $(OBJECTS_DIR)

$(LIBRARY): $(OBJECTS)
	$(RM) $(OBJECTS_DIR)$(DELIM)$@
	@echo Creating library $(@F) ...
	cd $(OBJECTS_DIR) && $(AR) $(ARFLAGS) -c $@ $(^F) && $(OBJDUMP) $(OBJDUMP_FLAGS) -o$@.sym $(^F)
	$(CP) $(OBJECTS_DIR)$(DELIM)$@ $(LIB_DIR_FORMATED) $(CMDQUIET)

$(EXECUTABLE): $(OBJECTS)
	$(RM) $(OBJECTS_DIR)$(DELIM)$@
	@echo Creating executable $(@F) ... in $(CURDIR)
	$(LD) -o $(OBJECTS_DIR)$(DELIM)$(EXECUTABLE) -m $(OBJECTS_DIR)$(DELIM)$(OUTPUT).map $(LINKER_SCRIPT) $(LDFLAGS)
	$(CP) $(OBJECTS_DIR)$(DELIM)$(EXECUTABLE) $(BIN_DIR_FORMATED) $(CMDQUIET)
	$(CP) $(OBJECTS_DIR)$(DELIM)$(OUTPUT).map $(BIN_DIR_FORMATED) $(CMDQUIET)
	$(SREC) -fi -o $(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).hex $(BIN_DIR_FORMATED)$(DELIM)$(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) $<

%.o: %.s
	$(CC) $(CFLAGS) $<
