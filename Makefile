# Compiler 
CXX_APPLE := clang++
CXX := /opt/homebrew/opt/llvm/bin/clang++

# Flags
CXXFLAGS := -std=c++20 -Wall -Wextra -g
LDFLAGS := 

# Directories
SRCDIR := src
OBJDIR := obj
BUILDDIR := build


# Files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
#DEPS := $(OBJECTS:.o=.d)	

TARGET := $(BUILDDIR)/main

# Default target 
all: $(TARGET)


# Link executable
$(TARGET): $(OBJECTS) | $(BUILDDIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files → object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Directory targets
$(OBJDIR) $(BUILDDIR):
	mkdir -p $@

#main: TestCpp/main.cpp
#	$(CXX) $(CXXFLAGS) TestCpp/main.cpp -o main --version

#main-apple: TestCpp/main.cpp
#	$(CXX_APPLE) $(CXXFLAGS) TestCpp/main.cpp -o main --version

print-compiler:
	@echo "CXX=$(CXX)"
	@$(CXX) --version | head -n 1

clean:
	rm -rf $(OBJDIR) $(BUILDDIR) 

.PHONY: all clean
