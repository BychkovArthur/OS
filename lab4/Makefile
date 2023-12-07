CC=gcc
FLAGS=-Wall -Werror -Wextra -Wpedantic
SRC=./src
INCLUDE=./include
BUILD=./build

build: clean lib
	$(CC) $(FLAGS) $(SRC)/main.c -o $(BUILD)/main_exe -L$(BUILD) -lFunction -lm

run: $(BUILD)/main_exe
	export LD_LIBRARY_PATH=$(BUILD)/; \
	$(BUILD)/main_exe

lib: $(SRC)/functions.c
	$(CC) $(FALAGS) -o $(BUILD)/libFunction.so -shared -fPIC $(SRC)/functions.c

clean:
	rm -f $(BUILD)/*.o $(BUILD)/*.so $(BUILD)/*_exe