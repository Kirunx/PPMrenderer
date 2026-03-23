CXX = g++
CFLAGS = -std=c++17 -O2
TARGET = ./build/PPMrndr
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CFLAGS) -o $(TARGET) $(SOURCES)

test: $(TARGET)
	$(TARGET)

clean:
	rm -rf build

.PHONY: all test clean