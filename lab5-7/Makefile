BUILD=./build

build: clean
	cd $(BUILD); cmake ..; make all;

client:
	$(BUILD)/client_exe

server:
	$(BUILD)/server_exe

clean:
	rm -rf $(BUILD)/*
