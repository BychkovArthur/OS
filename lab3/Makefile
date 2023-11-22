.PHONY: build run parent child clean

CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11 -fsanitize=address
BUILD=./build/
SRC=./src/

build: child parent
	

parent: $(SRC)parent.c $(SRC)functions.c
	$(CC) $(FLAGS) $(SRC)parent.c $(SRC)functions.c -o $(BUILD)parent_exe

child: $(SRC)child.c $(SRC)functions.c
	$(CC) $(FLAGS) $(SRC)child.c $(SRC)functions.c -o $(BUILD)child_exe

run:
	$(BUILD)parent_exe

clean:
	rm -rf $(BUILD)*_exe