# exporting from config/compiler-$(TARGET)-$(COMPILER).mk
export CC LD

# exporting from config/config.mk
export PROJECT TARGET COMPILER

# exporting from Makefile
export LIB_DIR BIN_DIR OUTPUT BUILD_DIR PROJECT_DIR BUILD_DIR_FORMATED BIN_DIR_FORMATED LIB_DIR_FORMATED

# exporting from host-$(HOST).mk
export RM_DIR RM CP MKDIR FIND DELIM CURRENT_DIR CMDQUIET

# exporting from all exports.mk
export GLOBAL_INCLUDE_DIR
