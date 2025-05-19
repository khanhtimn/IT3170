ifeq ($(OS),Windows_NT)
    COMPILER = cl
    EXE = .exe
    RM = rmdir /s /q $(TARGET_DIR)
    MKDIR = mkdir "$1" 2>nul || exit 0
else
    COMPILER = g++
    EXE =
    RM = rm -rf $(TARGET_DIR)
    MKDIR = mkdir -p "$1"
endif

CC = $(COMPILER)
CFLAGS = /std:c++17 /W4 /O2

TARGET_DIR = target

WEEK_DIRS = $(wildcard week*)

TARGET_WEEK_DIRS = $(patsubst week%,$(TARGET_DIR)/week%,$(notdir $(WEEK_DIRS)))

SOURCES = $(foreach dir,$(WEEK_DIRS),$(wildcard $(dir)/*.cpp))

TARGETS = $(patsubst %.cpp,$(TARGET_DIR)/%$(EXE),$(SOURCES))

all: create_dirs $(TARGETS)

create_dirs: $(TARGET_WEEK_DIRS)

$(TARGET_WEEK_DIRS):
	@$(call MKDIR,$@)
	@echo "Created directory $@"

$(TARGET_DIR)/%$(EXE): %.cpp
	@echo "Compiling $<..."
ifeq ($(OS),Windows_NT)
	@$(CC) $(CFLAGS) /Fe:"$@" /Fo:"$(dir $@)" $<
else
	@$(CC) $(CFLAGS) $< -o "$@"
endif

clean:
	@echo "Cleaning $(TARGET_DIR)..."
	@$(RM)

.PHONY: all clean create_dirs
