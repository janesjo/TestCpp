# Compiler Clang 22.1.0 Apple is just Clang++
CXX := /opt/homebrew/opt/llvm/bin/clang++

# Project
SRC_DIR := src
OBJ_DIR := obj
BUILD_DIR := build
TARGET := main

# Files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)

# Build type 
# Default: debug
CONFIG ?= debug


# -------------------------------------------------
# Config-specific settings
# -------------------------------------------------
ifeq ($(CONFIG),debug)
	CXXFLAGS := -std=c++20 -Wall -Wextra -O0 -g
	OBJ_SUBDIR := $(OBJ_DIR)/debug
	BIN_SUBDIR := $(BUILD_DIR)/debug
	LDFLAGS :=
endif


ifeq ($(CONFIG),release)
	CXXFLAGS := -std=c++20 -Wall -Wextra -O3 -DNDEBUG
	OBJ_SUBDIR := $(OBJ_DIR)/release
	BIN_SUBDIR := $(BUILD_DIR)/release
	LDFLAGS :=
endif


# -------------------------------------------------
# Derived paths
# -------------------------------------------------
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_SUBDIR)/%.o, $(SOURCES))
BIN := $(BIN_SUBDIR)/$(TARGET)


# -------------------------------------------------
# Default target
# -------------------------------------------------
all: $(BIN)


# -------------------------------------------------
# Debug / Release convenience targets
# -------------------------------------------------
debug:
	$(MAKE) CONFIG=debug

release:
	$(MAKE) CONFIG=release


# -------------------------------------------------
# Link
# -------------------------------------------------
$(BIN): $(OBJECTS) | $(BIN_SUBDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# -------------------------------------------------
# Compile
# -------------------------------------------------
$(OBJ_SUBDIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_SUBDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


# -------------------------------------------------
# Directory creation (order-only)
# -------------------------------------------------
$(OBJ_SUBDIR) $(BIN_SUBDIR):
	mkdir -p $@


# -------------------------------------------------
# Cleanup
# -------------------------------------------------
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)


print-compiler:
	@echo "CXX=$(CXX)"
	@$(CXX) --version | head -n 1

.PHONY: all clean
