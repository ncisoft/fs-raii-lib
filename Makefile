.PHONY : help init xinit clean build rebuild run
.PHONY : sync

help:
	@echo "Usage: make help | init | build | clean | run"
	@echo ""

clean: init
	scons -c
	rm -rf build
	@mkdir -p ./build
	cd build && cmake ..

init:
	@git submodule update --init --recursive
	@mkdir -p ./build
	@cd ./build && cmake .. && cmake ..


build: init
	@mkdir -p build
	cd build && cmake ..
	make -C ./build -j4

run: build
	./build/tests/ipc-bus-server
