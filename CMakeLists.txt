cmake_minimum_required(VERSION 3.2)
project(IPCBUS)

set(IPCBUS_VERSION_MAJOR 2)
set(IPCBUS_VERSION_MINOR 41)
set(IPCBUS_VERSION_DATABASE 135)
set(IPCBUS_VERSION_SOURCES_FILE 16)
set(IPCBUS_VERSION ${RTAGS_VERSION_MAJOR}.${RTAGS_VERSION_MINOR}.${RTAGS_VERSION_DATABASE})
set(IPCBUS_BINARY_ROOT_DIR ${PROJECT_BINARY_DIR})
# aux_source_directory(. DIR_SRCS)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CC 99)
set(CMAKE_CC True)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99  ")
#option(USE_DEBUG "Use DEBUG" ON)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif ()
# Detect linux
if (UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif ()

option(WITH_STATIC "build with static libraries." ON)
option(WITH_EMBEDDED_SRC "build with embedded src, tests source." ON)

set(LIBCORK_SOURCE
  libcork/src/libcork/cli/commands.c
  libcork/src/libcork/core/allocator.c
  libcork/src/libcork/core/error.c
  libcork/src/libcork/core/gc.c
  libcork/src/libcork/core/hash.c
  libcork/src/libcork/core/ip-address.c
  libcork/src/libcork/core/mempool.c
  libcork/src/libcork/core/timestamp.c
  libcork/src/libcork/core/u128.c
  libcork/src/libcork/core/version.c
  libcork/src/libcork/ds/array.c
  libcork/src/libcork/ds/bitset.c
  libcork/src/libcork/ds/buffer.c
  libcork/src/libcork/ds/dllist.c
  libcork/src/libcork/ds/file-stream.c
  libcork/src/libcork/ds/hash-table.c
  libcork/src/libcork/ds/managed-buffer.c
  libcork/src/libcork/ds/ring-buffer.c
  libcork/src/libcork/ds/slice.c
  libcork/src/libcork/posix/directory-walker.c
  libcork/src/libcork/posix/env.c
  libcork/src/libcork/posix/exec.c
  libcork/src/libcork/posix/files.c
  libcork/src/libcork/posix/process.c
  libcork/src/libcork/posix/subprocess.c
  libcork/src/libcork/pthreads/thread.c

        )
# ...
if (WITH_STATIC)
  add_library(cork STATIC ${LIBCORK_SOURCE})
  target_compile_definitions(cork PUBLIC -DCORK_API=CORK_LOCAL)
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

include_directories(libcork/build/include)
include_directories(libcork/include)
include_directories(contrib/liblogger)
include_directories(.xopt/include)
include_directories(./include)

# 添加 math 子目录
add_subdirectory(tests)
