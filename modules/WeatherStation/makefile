SOURCE += main.c

ifeq ($(COMPILER),cosmic)
SOURCE += vector.c
endif

SOURCE_DIR := source
INCLUDE_DIR := include

LINKER_SCRIPT := source/$(PROJECT).lkf
LIBRARY := WeatherStation

TARGET_LINK_LIBRARIES := StdPeriphDriver

ifeq ($(COMPILER),cosmic)
EXTENSION_EXE := sm8
else
EXTENSION_EXE := ihx
endif
EXECUTABLE := $(OUTPUT).$(EXTENSION_EXE)

include rules-$(COMPILER).mk
