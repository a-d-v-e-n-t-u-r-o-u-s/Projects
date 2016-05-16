SOURCE_DIR := source
INCLUDE_DIR := include


MOCKED_DIR:=$(MOCKED_DIR:%=$(PROJECT_DIR)/%)

ifeq (,$(strip $(MOCKED_HEADERS)))
MOCKED_HEADERS := $(foreach DIR, $(MOCKED_DIR), $(wildcard $(DIR)/*.h))
#$(info MOCKED_HEADERS: $(MOCKED_HEADERS))
else
MOCKED_HEADERS := $(MOCKED_HEADERS:%=$(MOCKED_DIR)/%)
#$(info Taking existing: $(MOCKED_HEADERS))
endif


CMOCK_ROOT := $(PROJECT_DIR)/framework/cmockgenerator/lib

CMOCK_RB := $(CMOCK_ROOT)/cmock.rb
#$(info CMOCK_RB: $(CMOCK_RB))


CMOCK_CONFIG_FILE := $(CMOCK_ROOT)/config.yml
#ifneq (,$(wildcard config.yml))
	#CMOCK_CONFIG_FILE := gcc_64.yml
#endif

SOURCE = $(addprefix mock, $(addsuffix .c,$(basename $(notdir $(MOCKED_HEADERS)))))
GEN_MOCKS := $(addprefix $(SOURCE_DIR)/, $(SOURCE))
#$(info GEN_MOCKS: $(GEN_MOCKS))

clean::
	-$(RM) source include

.NOTPARALLEL: $(GEN_MOCKS)

$(GEN_MOCKS):
	echo mockgen target
	echo config file is $(CMOCK_CONFIG_FILE)
	-$(RM) source include
	$(MKDIR) source include
	ruby $(CMOCK_RB) -o$(CMOCK_CONFIG_FILE) $(MOCKED_HEADERS)
	$(MV) source$(DELIM)*.h include
