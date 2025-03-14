ifeq ($(OS),Windows_NT)
	COMPILER = zig c++
    RM = del /q
    MKDIR = if not exist "$1" mkdir "$1"
    EXE = .exe
    TARGET_ABS = $(subst /,\,$(abspath $(TARGET_DIR)))
	DIRSEP = \\
else
	COMPILER = g++
    RM = rm -rf
    MKDIR = mkdir -p "$1"
    EXE =
    TARGET_ABS = $(abspath $(TARGET_DIR))
	DIRSEP = /
endif

CC = $(COMPILER)
CFLAGS = -std=c++17 -Wall -Wextra -O3
#Note the file descriptor when making new week
SOURCES = $(wildcard *.cpp)
EXECS = $(patsubst %.cpp,$(TARGET_DIR)$(DIRSEP)%$(EXE),$(SOURCES))

all: $(EXECS)

$(TARGET_DIR)$(DIRSEP)%$(EXE): %.cpp
	$(CC) $(CFLAGS) $< -o "$(TARGET_ABS)$(DIRSEP)$*$(EXE)"

.PHONY: all
