MODULE_1 := cpu
OBJ_FILES := main.o $(MODULE_1).o
INC_DIR := include
INC_FILES := $(MODULE_1).h
FLAGS := -Werror -Wpedantic -Wall -g
TARGET := gb_emulator

$(TARGET): build/$(MODULE_1).o build/main.o
	g++ $^ -o gb_emulator

build/$(MODULE_1).o: src/$(MODULE_1).cpp include/$(MODULE_1).h
	g++ $(FLAGS) -I include -c $< -o $@

build/main.o: src/main.cpp
	g++ $(FLAGS) -I include -c $< -o $@

clean:
	rm -rf build/*.o $(TARGET)