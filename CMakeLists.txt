cmake_minimum_required(VERSION 3.2)
project(IPCBUS)

set(IPCBUS_VERSION_MAJOR 2)
set(IPCBUS_VERSION_MINOR 41)
set(IPCBUS_VERSION_DATABASE 135)
set(IPCBUS_VERSION_SOURCES_FILE 16)
set(IPCBUS_VERSION ${RTAGS_VERSION_MAJOR}.${RTAGS_VERSION_MINOR}.${RTAGS_VERSION_DATABASE})
set(IPCBUS_BINARY_ROOT_DIR ${PROJECT_BINARY_DIR})
# aux_source_directory(. DIR_SRCS)

list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#INCLUDE (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/TestBigEndian.cmake)
include(CheckCSourceCompiles)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckLibraryExists)
include(CheckStructHasMember)
include(CheckSymbolExists)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)


configure_file(
  ${PROJECT_SOURCE_DIR}/include/config.h.cmake
  ${PROJECT_SOURCE_DIR}/include/ipc-bus/config.h
)

check_function_exists(vfork HAVE_VFORK)
check_function_exists(vfork HAVE_FORK)
check_function_exists(popen HAVE_POPEN)
check_function_exists(bzero HAVE_BZERO)

test_big_endian(WORDS_BIGENDIAN)
check_type_size(pid_t HAVE_PID_T)
check_type_size(uint64_t HAVE_UINT64_T)
check_type_size(uint32_t HAVE_UINT32_T)
check_type_size(uint16_t HAVE_UINT16_T)
check_type_size(uchar HAVE_UCHAR)
check_type_size(uhort HAVE_USHORT)
check_type_size(uint HAVE_UINT)
check_type_size(ulong HAVE_ULONG)
check_type_size(ulonglong HAVE_ULONGLONG)
check_type_size(voidptr HAVE_VOIDPTR)

check_type_size(uint_t HAVE_UINT_T)


set(FLAGS
  "${CMAKE_C_FLAGS}"
  "-Wall -Wno-unknown-pragmas"
  "-Wno-sign-compare"
  "-Wwrite-strings"
  "-Wno-unused"
  "-Wno-format-overflow"
  "-Wno-unused-variable"
  "-Wno-error=discarded-qualifiers"
  "-Wno-discarded-qualifiers"
  "-Wno-int-to-pointer-cast"
  "-Wno-pointer-to-int-cast"
)
list(JOIN FLAGS " " FLAGS2)

if(CMAKE_C_COMPILER_ID MATCHES GNU)
  set(CMAKE_C_FLAGS         "${FLAGS2}")
  set(CMAKE_C_FLAGS_DEBUG   "-O3 -g3")
  set(CMAKE_C_FLAGS_RELEASE "-O3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
  message(STATUS "optional:-std=c++11")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CC 99)
set(CMAKE_CC True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99  ")
#option(USE_DEBUG "Use DEBUG" ON)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()
# Detect linux
if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif ()

option(WITH_STATIC "build with static libraries." ON)
option(WITH_EMBEDDED_SRC "build with embedded src, tests source." ON)

set(LIBCORK_SOURCE
  contrib/libcork/src/libcork/cli/commands.c
  contrib/libcork/src/libcork/core/allocator.c
  contrib/libcork/src/libcork/core/error.c
  contrib/libcork/src/libcork/core/gc.c
  contrib/libcork/src/libcork/core/hash.c
  contrib/libcork/src/libcork/core/ip-address.c
  contrib/libcork/src/libcork/core/mempool.c
  contrib/libcork/src/libcork/core/timestamp.c
  contrib/libcork/src/libcork/core/u128.c
  contrib/libcork/src/libcork/core/version.c
  contrib/libcork/src/libcork/ds/array.c
  contrib/libcork/src/libcork/ds/bitset.c
  contrib/libcork/src/libcork/ds/buffer.c
  contrib/libcork/src/libcork/ds/dllist.c
  contrib/libcork/src/libcork/ds/file-stream.c
  contrib/libcork/src/libcork/ds/hash-table.c
  contrib/libcork/src/libcork/ds/managed-buffer.c
  contrib/libcork/src/libcork/ds/ring-buffer.c
  contrib/libcork/src/libcork/ds/slice.c
  contrib/libcork/src/libcork/posix/directory-walker.c
  contrib/libcork/src/libcork/posix/env.c
  contrib/libcork/src/libcork/posix/exec.c
  contrib/libcork/src/libcork/posix/files.c
  contrib/libcork/src/libcork/posix/process.c
  contrib/libcork/src/libcork/posix/subprocess.c
  contrib/libcork/src/libcork/pthreads/thread.c

        )
# ...
if (WITH_STATIC)
  add_library(cork STATIC ${LIBCORK_SOURCE})
  target_compile_definitions(cork PUBLIC -DCORK_API=CORK_LOCAL)
endif ()

set(LIBTASK_SOURCE
  ./contrib/libtask/asm.S
  ./contrib/libtask/channel.c
  ./contrib/libtask/context.c
  ./contrib/libtask/fd.c
  ./contrib/libtask/net.c
  ./contrib/libtask/print.c
  ./contrib/libtask/qlock.c
  ./contrib/libtask/rendez.c
  ./contrib/libtask/task.c
)

if (WITH_STATIC)
  add_library(task STATIC ${LIBTASK_SOURCE})
  target_compile_definitions(task PUBLIC)
endif ()

set(LIBLOGGER_SOURCE
  contrib/logger/logger.c
)

if (WITH_STATIC)
  add_library(logger STATIC ${LIBLOGGER_SOURCE})
  target_compile_definitions(logger PUBLIC)
endif ()

set(LIBCORE_SOURCE
  src/osclock.c
  src/network.c
  src/network.c
  src/fork.c
  src/raw-raii.c
  src/mpool.c
  )
if (WITH_STATIC)
    add_library(core STATIC ${LIBCORE_SOURCE})
    target_compile_definitions(core PUBLIC)
endif ()
# 加入一个配置头文件，用于处理 CMake 对源码的设置
# ...
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
  add_definitions(-Wall -Werror)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
  add_definitions(-Wall -Werror)
elseif(CMAKE_C_COMPILER_ID STREQUAL "Intel")
  add_definitions(-Wall -Werror)
endif(CMAKE_C_COMPILER_ID STREQUAL "GNU")

include_directories(contrib/libcork/include)
#include_directories(contrib/libcork/build/include)
include_directories(contrib/logger)
include_directories(contrib/libtask)
#include_directories(.xopt/include)
include_directories(include)

# 添加 math 子目录
add_subdirectory(tests)
