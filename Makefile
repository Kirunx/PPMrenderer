CXX = g++
CFLAGS = -std=c++17 -O3
TARGET = ./build/PPMrndr
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)

CONVERT = ffmpeg
INPUT = ./build/*.ppm

TIMESTAMP = $(shell date +%Y%m%d_%H%M%S)
OUTPUT = ./screenshots/screenshot_$(TIMESTAMP).png

convert:
	$(CONVERT) -y -i $(INPUT) -loglevel error $(OUTPUT)
	@echo "Conversion complete: $(OUTPUT)"

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	mkdir -p build
	$(CXX) $(CFLAGS) -o $(TARGET) $(SOURCES)

test: $(TARGET)
	$(TARGET)

clean:
	rm -rf build

.PHONY: all test clean