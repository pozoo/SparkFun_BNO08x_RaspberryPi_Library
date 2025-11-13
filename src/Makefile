# --- Configuration Variables ---
TARGET      = bno08x
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
OBJECTS     = $(patsubst %.cpp,%.o,$(CPP_SOURCES)) $(patsubst %.c,%.o,$(C_SOURCES))


# --- Build Rules ---

# Default target: build the final executable
all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	# Link all object files into the target executable
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Build successful!"

# Rule to compile C++ files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile C files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# --- Utility Rules ---

# Clean up generated object files and the executable
clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Cleanup complete."

.PHONY: all clean
