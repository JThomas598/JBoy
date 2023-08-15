SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(subst src,build,$(SRC_FILES:.cpp=.o))
FLAGS := -Werror -Wpedantic -Wall -g -D debug
TARGET := emu

$(TARGET): $(OBJ_FILES)
	g++ $^ -o $@

test/%.gb: test/%.o
	rgblink -o $@ $^
	rgbfix -v -p 0xFF $@

build/%.o: src/%.cpp
	g++ $(FLAGS) -I include -c $< -o $@

test/%.o: test/%.asm
	rgbasm -L -o $@ $^

clean:
	rm -rf build/*.o test/*.gb test/*.o $(TARGET)