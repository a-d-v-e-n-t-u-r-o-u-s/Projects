include framework.mk

.PHONY: all $(USED_FRAMEWORK)

all: $(USED_FRAMEWORK)

$(USED_FRAMEWORK):
	$(MAKE) -C $@ $(MAKEGOALS)

