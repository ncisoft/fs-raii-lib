# fs-raii-lib


ansi c (c99) compatible function-scope RAIIimplementation

Build tools:
* [CMake](https://cmake.org/)
* [Scons](https://scons.org/)
* [Ninja](https://ninja-build.org/)

Libaries:
* sudo apt-get install lua5.1
* sudo apt-get liblua5.1-0-dev
* sudo apt-get install libjemalloc-dev -y
* sudo apt-get install libclang-dev -y
* sudo apt-get install libcork-dev -y


Run Testcase:
make run

precondition:

You are able to check whether the code is conflict with using_raii_proot | using_raii_proot_complex_return by installing a patched
[luaclang-parser](https://github.com/ncisoft/luaclang-parser) .

run `make check_gc_root_new' to see what happens



