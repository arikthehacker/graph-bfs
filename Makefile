CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall
TARGET    = graph

all: $(TARGET)

$(TARGET): graph.cpp graph.h
	$(CXX) $(CXXFLAGS) -o $@ graph.cpp

run: $(TARGET)
	./$(TARGET) < sample-input.txt

test:
	./test.sh

clean:
	rm -f $(TARGET)

.PHONY: all run test clean
