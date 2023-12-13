.PHONY: clean test1 test2 run1 run2

SRC=./src
BUILD=./build
INCLUDE=./include
CC=gcc
# FLAGS=-Wall -Werror -Wextra -Wpedantic
FLAGS= 

test1: clean $(SRC)/resourceMapAllocatorTest.c $(SRC)/resourceMapAllocator.c $(SRC)/resourceMapBlockInfo.c
	$(CC) $(FLAGS) $(SRC)/resourceMapAllocatorTest.c $(SRC)/resourceMapAllocator.c $(SRC)/resourceMapBlockInfo.c -o $(BUILD)/test1_exe

run1:
	$(BUILD)/test1_exe

test2: clean $(SRC)/buddySystemAllocatorTest.c $(SRC)/buddySystemAllocator.c $(SRC)/buddySystemLinkedList.c
	$(CC) $(FLAGS) $(SRC)/buddySystemAllocatorTest.c $(SRC)/buddySystemAllocator.c $(SRC)/buddySystemLinkedList.c -o $(BUILD)/test2_exe

run2:
	$(BUILD)/test2_exe

clean:
	rm -f $(BUILD)/*_exe