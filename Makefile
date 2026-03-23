CXX = g++
CFLAGS = -std=c++17 -O2
TARGET = ./build/PPMrndr
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)

# Default rule
all: $(TARGET)

# Rule to create the build directory and compile
$(TARGET): $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CFLAGS) -o $(TARGET) $(SOURCES)

# Run the project
test: $(TARGET)
	$(TARGET)

# Clean up build artifacts
clean:
	rm -rf build

.PHONY: all test clean