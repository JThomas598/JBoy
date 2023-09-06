SRC_FILES_MODULES := $(wildcard src/modules/*.cpp)
SRC_FILES_DEBUG := $(wildcard src/debug/*.cpp)
OBJ_FILES_MODULES := $(subst src/modules,build/modules/obj,$(SRC_FILES_MODULES:.cpp=.o))
OBJ_FILES_DEBUG := $(subst src/debug, build/debug/obj,$(SRC_FILES_DEBUG:.cpp=.o))
FLAGS := -Werror -I include -Wpedantic -Wall
DEBUG_FLAGS := -g -D DEBUG
LIBS := -L lib -lSDL2 -lSDL2main
TARGET := emu

emu: $(OBJ_FILES_MODULES) build/core/obj/main.o
	g++ $^ $(LIBS) -o $@

debug: FLAGS += $(DEBUG_FLAGS) 
debug: debugger

debugger: $(OBJ_FILES_MODULES) $(OBJ_FILES_DEBUG) build/core/obj/debug_main.o
	g++ $^ $(LIBS) -o $@

%test: build/main/obj/memory.o build/test/cpp/obj/%_test.o build/main/obj/%.o
	g++  $^ $(LIBS) -o $@

%.gb: build/test/asm/obj/%.o
	rgblink -o $@ $^
	rgbfix -v -p 0xFF $@

build/modules/obj/%.o: src/modules/%.cpp
	g++ $(FLAGS) -I include -c $< -o $@

build/core/obj%.o: src/core/%.cpp
	g++ $(FLAGS) -I include -c $< -o $@

build/debug/obj%.o: src/debug/%.cpp
	g++ $(FLAGS) -I include -c $< -o $@

build/test/asm/obj/%.o: src/test/asm/%.asm
	rgbasm -L -o $@ $^

clean:
	rm -rf build/*/obj/* $(TARGET) *.exe