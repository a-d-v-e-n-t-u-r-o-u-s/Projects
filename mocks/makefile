include mocks.mk

.PHONY: all $(USED_MOCKS)

all: $(USED_MOCKS)

$(USED_MOCKS):
	$(MAKE) -C $@ $(MAKECMDGOALS)
