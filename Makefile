# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -O2
LDLIBS = -lcurl

# Output
TARGET = web-server

# File locations
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

# Clean rule
clean:
	rm -f $(TARGET)

.PHONY: all clean
