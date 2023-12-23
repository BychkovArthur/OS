BUILD=./build

build: clean
	cd $(BUILD); cmake ..; make all;

worker:
	$(BUILD)/worker_exe

server:
	$(BUILD)/server_exe

clean:
	rm -rf $(BUILD)/*
