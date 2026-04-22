CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pthread

SRC_DIR := TestCpp
SRC := $(wildcard $(SRC_DIR)/*.cpp)
TARGET := TestCpp
BUILD_DIR := bin

.PHONY: all build run clean

all: build

build: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: build
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
