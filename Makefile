.PHONY : help init xinit clean build rebuild run
.PHONY : sync cmake build-verbose ctags

help:
	@echo "Usage: make help | init | build | clean | run"
	@echo ""

clean: init
	rm -rf ./build
	@mkdir -p ./build
	@rm -rf build/CMakeFiles ./build/Makefile
	cd build && cmake .. && cmake ..

cmake:
	@mkdir -p ./build
	cd build && cmake .. && cmake ..

init:
	@git submodule update --init --recursive
	@mkdir -p ./build
	@cd ./build && cmake .. && cmake ..


build:
	@mkdir -p build
	cd build && cmake .. && cmake ..
	make -C ./build -j$(nproc)
	/usr/bin/ctags-exuberant --tag-relative=yes --langmap=c:.c.h -f tags src/*.[hc] tests/*.[hc] .xopt/include/*.h

build-verbose: init
	@mkdir -p build
	cd build && cmake ..
	make -C ./build VERBOSE=1

ctags: build
	/usr/bin/ctags-exuberant --tag-relative=yes --langmap=c:.c.h -f tags src/*.[hc] tests/*.[hc] .xopt/include/*.h
run: build
	./build/tests/ipc-bus-server
