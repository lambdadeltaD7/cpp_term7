CXX = g++

SRC = $(wildcard *.cpp)

CXX_FLAGS = -O3 -march=native -Wall -Wextra 
TARGET = build/main
OBJECTS = $(patsubst %.cpp,build/%.o,$(SRC))
DEP = $(OBJECTS:.o=.d)

DEBUG_FLAGS = -O0 -g3 -Wall -Wextra 
DEBUG_TARGET = build/debug/main
DEBUG_OBJECTS = $(patsubst %.cpp,build/debug/%.o,$(SRC))
DEBUG_DEP = $(DEBUG_OBJECTS:.o=.d)


all: $(TARGET)

debug: $(DEBUG_TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

build/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -MP -c $< -o $@


$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CXX) $(DEBUG_OBJECTS) -o $@

build/debug/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(DEBUG_FLAGS) -MMD -MP -c $< -o $@


clean:
	rm -rf build


.PHONY: all debug clean

-include $(DEP) $(DEBUG_DEP)

