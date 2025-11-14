# Top-level Makefile for SparkFun BNO08x Raspberry Pi Library
# Targets:
#   make cpp      -> build C++ executable (in src/out/bno08x)
#   make python   -> build Python extension module (python/bno08x*.so)
#   make all      -> build both
#   make clean    -> clean C++ and Python build artifacts
#   make clean-cpp / clean-python -> granular cleans

# --- Configuration ---
SRCDIR      := src
PYDIR       := python
OUTDIR      := $(SRCDIR)/out
TARGET_CPP  := $(OUTDIR)/bno08x
PY_MODULE   := $(PYDIR)/bno08x$(shell python3 -c 'import sysconfig;print(sysconfig.get_config_var("EXT_SUFFIX"))')
PYBIND_SRC  := $(PYDIR)/bindings.cpp

CXX         := g++
CC          := gcc
CXXFLAGS    := -Wall -O2 -std=c++17
CFLAGS      := -Wall -O2 -std=c11
LDFLAGS     := -lgpiod
PY_CXXFLAGS := -O3 -Wall -shared -std=c++17 -fPIC $(shell python3 -m pybind11 --includes)
PY_LDFLAGS  := -lgpiod

# Source discovery for C++ / C
CPP_SOURCES := $(wildcard $(SRCDIR)/*.cpp)
C_SOURCES   := $(wildcard $(SRCDIR)/*.c)
# Object files for core C++/C (exclude bno086.cpp if needed? keep all for simplicity)
CPP_OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OUTDIR)/%.o,$(CPP_SOURCES))
C_OBJECTS   := $(patsubst $(SRCDIR)/%.c,$(OUTDIR)/%.o,$(C_SOURCES))
OBJECTS     := $(CPP_OBJECTS) $(C_OBJECTS)

# Default target
.PHONY: all
all: cpp python

# --- C++ executable ---
.PHONY: cpp
cpp: $(TARGET_CPP)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(OUTDIR)/%.o: $(SRCDIR)/%.cpp | $(OUTDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OUTDIR)/%.o: $(SRCDIR)/%.c | $(OUTDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET_CPP): $(OBJECTS)
	@echo "Linking C++ executable: $@"
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "C++ build complete."

# --- Python extension module ---
.PHONY: python
python: $(PY_MODULE)

$(PY_MODULE): $(PYBIND_SRC) $(OBJECTS)
	@echo "Building Python extension: $@"
	$(CXX) $(PY_CXXFLAGS) $(PYBIND_SRC) $(OBJECTS) -o $@ $(PY_LDFLAGS)
	@echo "Python module built at $@"

# --- Cleaning ---
.PHONY: clean clean-cpp clean-python
clean: clean-cpp clean-python

clean-cpp:
	@echo "Cleaning C++ objects and executable";
	rm -rf $(OUTDIR)

clean-python:
	@echo "Cleaning Python extension";
	rm -f $(PY_MODULE)

# Convenience info target
.PHONY: info
info:
	@echo "Executable:    $(TARGET_CPP)";
	@echo "Python module: $(PY_MODULE)";
	@echo "C++ sources:   $(CPP_SOURCES)";
	@echo "C sources:     $(C_SOURCES)";
