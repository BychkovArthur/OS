.PHONY: build run

CC=gcc
FLAGS=-Wall -Werror -Wextra -std=c11 -fsanitize=address
BUILD=./build/
SRC=./src/

build:
	$(CC) $(FLAGS) $(SRC)parent.c $(SRC)functions.c -o $(BUILD)parent

run:
	$(BUILD)parent