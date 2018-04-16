SOURCE += main.c

ifeq ($(COMPILER),cosmic)
SOURCE += vector.c
endif

SOURCE_DIR := source
INCLUDE_DIR := include

LINKER_SCRIPT := source/$(PROJECT).lkf
LIBRARY := RemoteControl

TARGET_LINK_LIBRARIES := System
TARGET_LINK_LIBRARIES += StdPeriphDriver
TARGET_LINK_LIBRARIES += display

ifeq ($(COMPILER),cosmic)
EXTENSION_EXE := sm8
else
EXTENSION_EXE := ihx
endif
EXECUTABLE := $(OUTPUT).$(EXTENSION_EXE)

include rules-$(COMPILER).mk
