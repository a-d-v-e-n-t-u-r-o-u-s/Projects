
MAKE_VERSION_REQUIRED := 3.81

ifneq ($(filter $(MAKE_VERSION_REQUIRED),$(MAKE_VERSION)),$(MAKE_VERSION_REQUIRED))
$(error You are using $(MAKE_VERSION) when you shall use the $(MAKE_VERSION_REQUIRED))
endif

.DEFAULT_GOAL := all

include config/config.mk

3RDPARTY_DIR:= 3rdparty
DRIVERS_DIR:= drivers
MODULES_DIR:= modules

include projects/$(PROJECT)/version.mk
include projects/$(PROJECT)/3rdparty.mk

3RDPARTYS_EXPORTS :=$(patsubst %,$(3RDPARTY_DIR)/%/exports.mk,$(USED_3RDPARTY))

include projects/$(PROJECT)/drivers.mk

DRIVERS_EXPORTS :=$(patsubst %,$(DRIVERS_DIR)/%/exports.mk,$(USED_DRIVERS))

include projects/$(PROJECT)/modules.mk

MODULES_EXPORTS :=$(patsubst %,$(MODULES_DIR)/%/exports.mk,$(USED_MODULES))

include $(3RDPARTYS_EXPORTS) $(DRIVERS_EXPORTS) $(MODULES_EXPORTS)

OUTPUT := $(PROJECT)-$(VERSION_MAJOR)_$(VERSION_MINOR)_$(VERSION_PATCH)
PROJECT_DIR := $(CURDIR)
BUILD_DIR := $(CURDIR)/build
OUTPUT_DIR := $(CURDIR)/bin
BIN_DIR := $(BUILD_DIR)/bin
LIB_DIR := $(BUILD_DIR)/libs

MAKEFLAGS:= -I$(CURDIR) -I$(CURDIR)/config -I$(CURDIR)/projects/$(PROJECT) -R

include config/compiler-$(TARGET)-$(COMPILER).mk
include config/exports.mk

.PHONY: all flash clean $(3RDPARTY_DIR) $(DRIVERS_DIR) $(MODULES_DIR)

all: banner $(OUTPUT_DIR) executable done

flash:
	stm8flash -cstlink -s 0x8000 -pstm8l150 -w build/bin/$(OUTPUT).ihx


$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(LIB_DIR)

executable: $(OUTPUT)
	@echo $(OUTPUT)

$(OUTPUT): $(3RDPARTY_DIR) $(DRIVERS_DIR) $(MODULES_DIR)

$(3RDPARTY_DIR):
	@echo making $@
	@echo $(3RDPARTYS_EXPORTS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(DRIVERS_DIR):
	@echo making $@
	@echo $(DRIVERS_EXPORTS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(MODULES_DIR):
	@echo making $@
	@echo $(MODULES_EXPORTS)
	$(MAKE) -C $@ $(MAKECMDGOALS)

debug:
	@echo       3RDPARTYS_EXPORTS: $(3RDPARTYS_EXPORTS)
	@echo       DRIVERS_EXPORTS: $(DRIVERS_EXPORTS)
	@echo       MODULES_EXPORTS: $(MODULES_EXPORTS)

banner:
	@echo -------------------------------------------------------------------------------
	@echo
	@echo         PROJECT: $(PROJECT)
	@echo       MAKEGOALS: $(MAKEGOALS)
	@echo       TARGET_OS: $(TARGET_OS)
	@echo        COMPILER: $(CC) $(CCVERSION)
	@echo          OUTPUT: $(OUTPUT)
	@echo
	@echo -------------------------------------------------------------------------------

done:
	@echo -------------------------------------------------------------------------------
	@echo Through the Force the binary files I build.
	@echo
	@echo          OUTPUT: $(OUTPUT)
	@echo        RAM :
	@echo      FLASH :
	@echo
	@echo May the Force be with you!!!
	@echo -------------------------------------------------------------------------------

clean:
	@echo ------------------------------------------------------------------------------
	@echo
	@echo         Performing clean:
	@echo
	@echo ------------------------------------------------------------------------------
	-rm -rf $(BUILD_DIR)
	-rm -rf $(OUTPUT_DIR)
