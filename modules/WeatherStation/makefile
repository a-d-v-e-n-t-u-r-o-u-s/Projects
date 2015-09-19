SOURCE += main.c

ifeq ($(COMPILER),cosmic)
SOURCE += vector.c
endif

SOURCE_DIR := source
INCLUDE_DIR := include

LINKER_SCRIPT := source/$(PROJECT).lkf
LIBRARY := WeatherStation

TARGET_LINK_LIBRARIES := StdPeriphDriver

EXTENSION_EXE := sm8
EXECUTABLE := $(OUTPUT).$(EXTENSION_EXE)

include rules-$(COMPILER).mk
