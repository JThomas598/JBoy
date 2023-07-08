MODULE_1 := registers
MODULE_1_TEST := cputest
OBJ_FILES := main.o $(MODULE_1).o
INC_FILES := $(MODULE_1).h
INC_DIR := include
FLAGS := -Werror -Wpedantic -Wall
TARGET := gb_emulator

$(TARGET): build/$(MODULE_1).o build/main.o
	g++ $^ -o gb_emulator

build/$(MODULE_1).o: src/$(MODULE_1).cpp include/$(MODULE_1).h
	g++ $(FLAGS) -I include -c $< -o $@

build/main.o: src/main.cpp
	g++ $(FLAGS) -I include -c $< -o $@

clean:
	rm -rf build/*.o $(TARGET)