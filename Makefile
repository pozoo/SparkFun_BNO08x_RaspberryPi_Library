# Compiler settings
CXX = g++
CC = gcc
CXXFLAGS = -Wall -std=c++11 -Isrc -Isrc/sh2
CFLAGS = -Wall -Isrc -Isrc/sh2
LDFLAGS = -lm -lpthread

# Directories
SRC_DIR = src
BUILD_DIR = build
TARGET = bno08x_example

# Source files
CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/sh2/*.cpp)
C_SOURCES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/sh2/*.c)

# Object files
CPP_OBJECTS = $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
C_OBJECTS = $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS)

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
    mkdir -p $(BUILD_DIR)
    mkdir -p $(BUILD_DIR)/sh2

# Link
$(TARGET): $(OBJECTS)
    $(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile C++ files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
    rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean