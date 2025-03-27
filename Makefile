ifeq ($(OS),Windows_NT)
    COMPILER = zig c++
    EXE = .exe
    RM = powershell -Command "if (Test-Path '$(TARGET_DIR)') { Remove-Item -Recurse -Force '$(TARGET_DIR)' }"
    MKDIR = powershell -Command "if (-not (Test-Path '$(subst /,\,$1)')) { New-Item -ItemType Directory -Force -Path '$(subst /,\,$1)' }"
else
    COMPILER = g++
    EXE =
    RM = rm -rf $(TARGET_DIR)
    MKDIR = mkdir -p "$1"
endif

CC = $(COMPILER)
CFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -O3

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
	@$(CC) $(CFLAGS) $< -o "$@"

clean:
	@echo "Cleaning $(TARGET_DIR)..."
	@$(RM)

.PHONY: all clean create_dirs
