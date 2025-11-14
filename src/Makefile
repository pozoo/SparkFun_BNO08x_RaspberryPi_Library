# --- Configuration Variables ---
TARGET      = bno08x
CWD         = $(shell pwd)
OUT_DIR     = out
CXX         = g++
CC          = gcc
CXXFLAGS    = -Wall -O2 -std=c++17
CFLAGS      = -Wall -O2 -std=c11
# Linker flags (Crucial for Raspberry Pi GPIO library)
LDFLAGS     = -lgpiod

# --- File Discovery ---
CPP_SOURCES = $(wildcard *.cpp)
C_SOURCES   = $(wildcard *.c)
SOURCES     = $(CPP_SOURCES) $(C_SOURCES)

# Generate list of object files (.o) from source files (.cpp and .c)
# We use 'patsubst' to safely substitute suffixes without recursion
OBJECTS     = $(patsubst %.cpp,$(OUT_DIR)/%.o,$(CPP_SOURCES)) \
			  $(patsubst %.c,$(OUT_DIR)/%.o,$(C_SOURCES))


# --- Build Rules ---

# Default target: build the final executable
all: $(OUT_DIR)/$(TARGET)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT_DIR)/$(TARGET): $(OUT_DIR) $(OBJECTS)
	@echo "Linking $(TARGET)..."
	# Link all object files into the target executable
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful! Binary at $(OUT_DIR)/$(TARGET)"

# Rule to compile C++ files into object files
$(OUT_DIR)/%.o: %.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile C files into object files
$(OUT_DIR)/%.o: %.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Utility Rules ---

# Clean up generated object files and the executable
clean:
	@echo "Cleaning up..."
	rm -rf $(OUT_DIR)
	@echo "Cleanup complete."

.PHONY: all clean
