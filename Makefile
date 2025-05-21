ifeq ($(OS),Windows_NT)
    CC = cl
    EXE = .exe
    RM = rmdir /s /q
    MKDIR = mkdir
    CFLAGS = /std:c++17 /W4 /O2 /EHsc
    COMPILE = $(CC) $(CFLAGS) /Fo"$(dir $@)" /Fe"$@" $<
    LINK = $(CC) $(CFLAGS) /Fo"$(dir $@)" /Fe"$@" $^
else
    CC = gcc
    EXE =
    RM = rm -rf
    MKDIR = mkdir -p
    CFLAGS = -std=c++17 -Wall -O2 -x c++
    LDFLAGS ?= -lstdc++ -lm
    COMPILE = $(CC) $(CFLAGS) $< -o "$@" $(LDFLAGS)
    LINK = $(CC) $(CFLAGS) $^ -o "$@" $(LDFLAGS)
endif

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/exercises/*.cpp)
TARGETS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%$(EXE),$(SOURCES))

TEST_EXE = $(BUILD_DIR)/test$(EXE)
TEST_DIR = tests
TEST_SOURCES = $(SRC_DIR)/testing/test.cpp \
               $(SRC_DIR)/testing/test_runner.cpp \
               $(SRC_DIR)/testing/test_parser.cpp \
               $(SRC_DIR)/testing/test_result.cpp \
               $(SRC_DIR)/testing/output_formatter.cpp \
               $(SRC_DIR)/testing/process_executor.cpp

.PHONY: all
all: $(TARGETS) $(TEST_EXE)

$(BUILD_DIR)/%$(EXE): $(SRC_DIR)/%.cpp
	@$(MKDIR) "$(dir $@)" 2>nul || exit 0
	@echo "Compiling $<..."
ifeq ($(OS),Windows_NT)
	@$(COMPILE)
else
	@$(COMPILE)
endif

$(TEST_EXE): $(TEST_SOURCES)
	@$(MKDIR) "$(dir $@)" 2>nul || exit 0
	@echo "Compiling test framework..."
ifeq ($(OS),Windows_NT)
	@$(LINK)
else
	@$(LINK)
endif

.PHONY: test test-all
test: $(BUILD_DIR)/$(word 2,$(MAKECMDGOALS))/$(word 3,$(MAKECMDGOALS))$(EXE) $(TEST_EXE)
	$(TEST_EXE) $(word 2,$(MAKECMDGOALS)) $(word 3,$(MAKECMDGOALS))

%:
	@:

test-all: $(TARGETS) $(TEST_EXE)
ifeq ($(OS),Windows_NT)
	@for /d %%w in ($(SRC_DIR)\week*) do @for %%p in (%%w\*.cpp) do @echo. && echo Testing %%~nw/%%~np... && "$(TEST_EXE)" %%~nw %%~np
else
	@for src in $(SOURCES); do \
		week=$$(basename $$(dirname $$src)); \
		prog=$$(basename $$src .cpp); \
		echo "\nTesting $$week/$$prog..."; \
		$(TEST_EXE) $$week $$prog; \
	done
endif

.PHONY: clean
clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(BUILD_DIR)" (echo "Cleaning $(BUILD_DIR)..." && $(RM) "$(BUILD_DIR)")
	@if exist "$(TEST_DIR)" (for /r "$(TEST_DIR)" %%f in (*.out) do del "%%f")
else
	@if [ -d "$(BUILD_DIR)" ]; then \
		echo "Cleaning $(BUILD_DIR)..."; \
		$(RM) "$(BUILD_DIR)"; \
	fi
	@find "$(TEST_DIR)" -name "*.out" -type f -delete
endif
