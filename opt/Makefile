CXX = g++

SRC = $(filter-out main.cpp gtk_ex.cpp,$(wildcard *.cpp))
COMMON_OBJECTS = $(patsubst %.cpp,build/%.o,$(SRC))

CXX_FLAGS = -O3 -march=native -Wall -Wextra 
DEBUG_FLAGS = -O0 -g3 -Wall -Wextra 

CLI_TARGET = build/cli
CLI_OBJECTS = build/main.o $(COMMON_OBJECTS) 
CLI_DEP = $(CLI_OBJECTS:.o=.d)

GTK_TARGET = build/gtk
GTK_OBJECTS = build/gtk_ex.o $(COMMON_OBJECTS) 
GTK_DEP = $(GTK_OBJECTS:.o=.d)
GTK_FLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)


DEBUG_TARGET = build/debug/cli
DEBUG_OBJECTS = $(CLI_OBJECTS)
DEBUG_DEP = $(DEBUG_OBJECTS:.o=.d)


cli: $(CLI_TARGET)

gtk: $(GTK_TARGET)

debug: $(DEBUG_TARGET)


build/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) -MMD -MP -c $< -o $@


$(CLI_TARGET): $(CLI_OBJECTS)
	$(CXX) $(CLI_OBJECTS) -o $@


$(GTK_TARGET): $(GTK_OBJECTS)
	$(CXX) $(GTK_OBJECTS) $(GTK_LIBS) -o $@

build/gtk_ex.o: gtk_ex.cpp
	mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(GTK_FLAGS) -MMD -MP -c $< -o $@


$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	$(CXX) $(DEBUG_OBJECTS) -o $@

build/debug/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(DEBUG_FLAGS) -MMD -MP -c $< -o $@


clean:
	rm -rf build


.PHONY: all debug clean

-include $(CLI_DEP) $(GTK_DEP) $(DEBUG_DEP)

