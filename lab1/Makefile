.PHONY: build run clean

build: clean
	mkdir ./build;
	cd ./build; pwd; cmake ./..; make all;

run:
	./build/parent_exe;

clean:
	rm -rf ./build/;