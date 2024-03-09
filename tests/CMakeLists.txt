# redir need linux/* stuff
if (LINUX)
    option(WITH_IPCBUS "Build ipc-bus-server" ON)
endif ()


set(IPCBUS_SOURCE
  ipc-bus-server.c
        )

if (WITH_STATIC)
find_library(LIBEV libev.a)

# Dependencies we need for static and shared
endif ()

set(T_SOURCE
  t.c
        )

set(TYPES_TEST_SOURCE
  types-test.c
)

set(MRAII_TEST_SOURCE
  mraii-test.c
)

set(TESTDELAY_SOURCE
  testdelay.c
)

set(CHECK_SOURCE
  check-file-zero.c
)

set(JSON_SOURCE
  #json-test.c
)

set(FS_RAII_TEST_SOURCE
  fs_raii-test.c
)


if (WITH_STATIC)
find_library(LIBEV libev.a)
find_library(LIBCJSON libcjson.a)
find_library(LIBCORE libcore.a)
#find_library(LIBMELON libmelon.a)

# Dependencies we need for static and shared
endif ()

find_library(LIBEV_SHARED ev)


if (WITH_STATIC)
# ------------------------------------------------------------------
# Static
# By default we use normal name for static, all shared targets will add a `-shared' suffix

find_library(dl NAMES dl PATH /usr/lib/x86_64-linux-gnu/libdl.so)
set(_STDLIBS
  logger
  core
  ev
  cork
  cjson
#  melon
  task
  pthread
  dl
)
list(JOIN _STDLIBS " " STDLIBS)

add_executable(ipc-bus-server ${IPCBUS_SOURCE})
add_executable(t.exe ${T_SOURCE})
#add_executable(json-test.exe ${JSON_SOURCE})
add_executable(testdelay.exe ${TESTDELAY_SOURCE})
add_executable(mraii-test.exe ${MRAII_TEST_SOURCE})
add_executable(fs-raii-test.exe ${FS_RAII_TEST_SOURCE})
add_executable(types-test.exe ${TYPES_TEST_SOURCE})
add_executable(check-file-zero.exe ${CHECK_SOURCE})

target_link_libraries(ipc-bus-server
  logger
  core
  ev
  cork
  cjson
#  melon
  task
  pthread
)

target_link_libraries(t.exe "${_STDLIBS}"
)

target_link_libraries(types-test.exe "${_STDLIBS}"
)

#target_link_libraries(json-test.exe "${_STDLIBS}"
#)

target_link_libraries(testdelay.exe "${_STDLIBS}"
)

target_link_libraries(mraii-test.exe "${_STDLIBS}"
)

target_link_libraries(fs-raii-test.exe  "${_STDLIBS}"
)
target_link_libraries(check-file-zero.exe "${_STDLIBS}" ${CMAKE_DL_LIBS}
)

endif ()

# ------------------------------------------------------------------
# Shared
# Shared target will have a `-shared' suffix
# All shared dependency also have `-shared' suffix
# For shared binary, we still use the same name as static, without `-shared', but will output to shared directory
#set_target_properties(ss-server-shared ss-tunnel-shared ss-s5forwarder-shared ss-manager-shared ss-local-shared ss-redir-shared


# ------------------------------------------------------------------
# Misc
# Recommend to install shared by default
add_custom_target(distclean
        COMMAND ${CMAKE_COMMAND} -E echo WARNING: distclean target is not functional
        COMMAND ${CMAKE_COMMAND} -E echo Use 'git clean -fdx' instead
        VERBATIM
        )
