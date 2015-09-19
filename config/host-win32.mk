RM_DIR := rmdir /Q /S
RM := del /Q
CP := copy
MKDIR := mkdir
DELIM := \ 
DELIM := $(strip $(DELIM))
CMDQUIET := >nul 2>nul & verify>nul
