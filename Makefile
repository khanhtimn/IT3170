ifeq ($(OS),Windows_NT)
    RM = del /q
    MKDIR = mkdir
    TARGET_PATH = ..\$(TARGET_DIR)\week
else
    RM = rm -rf
    MKDIR = mkdir -p
    TARGET_PATH = ../$(TARGET_DIR)/week
endif

TARGET_DIR = target

# Week directories
WEEKS = $(wildcard week*)

all: $(WEEKS)

$(WEEKS):
	@echo "Building targets for $@..."
	$(MAKE) -C $@ TARGET_DIR="$(TARGET_PATH)$(@:week%=%)"

.PHONY: all $(WEEKS)
