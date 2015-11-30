include compiler-$(TARGET)-$(COMPILER).mk

CURDIR_RELATIVE := $(subst $(PROJECT_DIR),,$(CURDIR))
OBJECTS_DIR := $(BUILD_DIR)$(CURDIR_RELATIVE)
OBJECTS_DIR := $(subst /,$(DELIM),$(OBJECTS_DIR))
OBJECTS_DIR_RELATIVE := $(subst /,$(DELIM),$(patsubst %,../..%,$(subst $(PROJECT_DIR_FORMATED),,$(OBJECTS_DIR))))

OBJECTS := $(addsuffix .o,$(basename $(SOURCE)))

INCLUDE_DIR += ../../projects/$(PROJECT) $(patsubst %,../../%,$(GLOBAL_INCLUDE_DIR))

vpath
vpath %.c 		$(subst  ,:,$(SOURCE_DIR))
vpath %.s 		$(subst  ,:,$(SOURCE_DIR))
vpath %.o 		$(OBJECTS_DIR)
vpath %.d 		$(DEP_DIR)
vpath %.$(LIBEXT) 	$(LIB_DIR)

.SUFFIXES:
.SUFFIXES: .c .o .s .d

.PHONY: all

LIBRARY := $(if $(LIBRARY),$(LIBPREFIX)$(LIBRARY)$(LIBEXT))

all: $(OBJECTS_DIR_RELATIVE) $(LIBRARY) $(EXECUTABLE)

$(OBJECTS_DIR_RELATIVE):
	@echo Creating object directory ...
	$(MKDIR) $(OBJECTS_DIR_RELATIVE)

$(LIBRARY): $(OBJECTS)
	$(RM) $(OBJECTS_DIR_RELATIVE)$(DELIM)$@
	@echo Creating library $(@F) ...
	cd $(OBJECTS_DIR_RELATIVE) && $(AR) $(ARFLAGS) -c $@ $(^F) && $(OBJDUMP) $(OBJDUMP_FLAGS) -o$@.sym $(^F)
	$(MV) $(OBJECTS_DIR_RELATIVE)$(DELIM)$@ $(LIB_DIR_FORMATED) $(CMDQUIET)

$(EXECUTABLE): $(OBJECTS)
	$(RM) $(OBJECTS_DIR_RELATIVE)$(DELIM)$@
	@echo Creating executable $(@F) ... in $(CURDIR)
	$(LD) -o $(OBJECTS_DIR_RELATIVE)$(DELIM)$(EXECUTABLE) -m $(OBJECTS_DIR_RELATIVE)$(DELIM)$(OUTPUT).map $(LINKER_SCRIPT) $(LDFLAGS)
	$(CP) $(OBJECTS_DIR_RELATIVE)$(DELIM)$(EXECUTABLE) $(BIN_DIR_FORMATED) $(CMDQUIET)
	$(CP) $(OBJECTS_DIR_RELATIVE)$(DELIM)$(OUTPUT).map $(BIN_DIR_FORMATED) $(CMDQUIET)
	$(FIND) "segment" $(OBJECTS_DIR_RELATIVE)$(DELIM)$(OUTPUT).map
	$(SREC) -fi -o $(BIN_DIR_FORMATED)$(DELIM)$(OUTPUT).hex $(BIN_DIR_FORMATED)$(DELIM)$(EXECUTABLE)

%.o: %.c
	$(CC) $(CFLAGS) $<
	$(CC) $(CFLAGS) -sm $< > $(DEP_DIR_FORMATED)$(DELIM)$(basename $(notdir $(<))).d 2>&1

%.o: %.s
	$(CC) $(CFLAGS) $<
