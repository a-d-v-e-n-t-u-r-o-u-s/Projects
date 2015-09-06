include compiler-$(TARGET)-$(COMPILER).mk

CURDIR_RELATIVE := $(subst $(PROJECT_DIR),,$(CURDIR))
OBJECTS_DIR := $(BUILD_DIR)$(CURDIR_RELATIVE)

# maybe pathsubst is better here
OBJECTS := $(addsuffix .rel,$(basename $(SOURCE)))

INCLUDE_DIR += ../../projects/$(PROJECT) $(patsubst %,../../%,$(GLOBAL_INCLUDE_DIR))

vpath
vpath %.c 		$(subst  ,:,$(SOURCE_DIR))
vpath %.s 		$(subst  ,:,$(SOURCE_DIR))
vpath %.rel 		$(OBJECTS_DIR)

.SUFFIXES:
.SUFFIXES: .c .s .rel

.PHONY: all

LIBRARY := $(if $(LIBRARY),$(LIBPREFIX)$(LIBRARY)$(LIBEXT))

all: $(OBJECTS_DIR) $(LIBRARY) $(EXECUTABLE)

$(OBJECTS_DIR):
	@echo Creating object directory ...
	mkdir -p $(OBJECTS_DIR)

$(OBJECTS): $(OBJECTS_DIR)

$(LIBRARY): $(OBJECTS)
	-rm -rf $(OBJECTS_DIR)/$@
	@echo Creating library $(@F) ...
	cd $(OBJECTS_DIR) && $(AR) $(ARFLAGS) -rc $@.lib $(^F)
	cp $(OBJECTS_DIR)/$@.lib $(LIB_DIR)

$(EXECUTABLE): $(OBJECTS)
	-rm -rf $(OBJECTS_DIR)/$@
	@echo Creating executable $(@F) ... in $(CURDIR)
	$(LD) $(LDFLAGS) $(patsubst %,$(OBJECTS_DIR)/%,$(OBJECTS)) -L$(LIB_DIR) \
	$(TARGET_LINK_LIBRARIES) -o $(BIN_DIR)/$(EXECUTABLE)

%.rel: %.s
	@echo $(<F)
	$(CC) $(CFLAGS) $(CURDIR)/$<

%.rel: %.c
	@echo $(<F)
	$(CC) $(CFLAGS) $(CURDIR)/$<
