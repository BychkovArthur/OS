CC=gcc
FLAGS=-Wall -Werror -Wextra -Wpedantic
SRC=./src
INCLUDE=./include
BUILD=./build

build1: clean functions lib1
	$(CC) $(FLAGS) $(SRC)/compileTimeLinking1.c -o $(BUILD)/main_exe -L$(BUILD) -lFunctions -lMyLib1 -lm

build2: clean functions lib2
	$(CC) $(FLAGS) $(SRC)/compileTimeLinking2.c -o $(BUILD)/main_exe -L$(BUILD) -lFunctions -lMyLib2 -lm

build3: clean functions lib1 lib2
	$(CC) $(FLAGS) $(SRC)/runTimeLinking.c -o $(BUILD)/main_exe -L$(BUILD) -lFunctions -lm -ldl

run: $(BUILD)/main_exe
	export LD_LIBRARY_PATH=$(BUILD)/; \
	$(BUILD)/main_exe

# Libraries
lib1: $(SRC)/myLib1.c
	$(CC) $(FALAGS) -o $(BUILD)/libMyLib1.so -shared -fPIC $(SRC)/myLib1.c

lib2: $(SRC)/myLib2.c
	$(CC) $(FALAGS) -o $(BUILD)/libMyLib2.so -shared -fPIC $(SRC)/myLib2.c

functions: $(SRC)/functions.c
	$(CC) $(FALAGS) -o $(BUILD)/libFunctions.so -shared -fPIC $(SRC)/functions.c

clean:
	rm -f $(BUILD)/*.o $(BUILD)/*.so $(BUILD)/*_exe