include modules.mk

.PHONY: all $(MAIN_MODULE) $(USED_MODULES)

all: $(MAIN_MODULE)

$(MAIN_MODULE) : $(USED_MODULES)
	@echo MAIN modules : $@
	@echo $(MAKE) -C $@ $(MAKECMDGOALS)
	@$(MAKE) -C $@ $(MAKECMDGOALS)

$(USED_MODULES):
	@echo Used modules : $@
	@echo $(MAKE) -C $@ $(MAKECMDGOALS)
	@$(MAKE) -C $@ $(MAKECMDGOALS)
