CXX = g++
CXX_FLAGS = -O3 -march=native -Wall -Wextra 

TARGET = build/main
SRC = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp,build/%.o,$(SRC))
DEP = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

build/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	rm -rf build


