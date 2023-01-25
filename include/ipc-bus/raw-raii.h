#pragma once

#include <ipc-bus/types.h>

extern void voidptr_raii_close(voidptr *data);
extern void fd_raii_close(int *pfd);

#define declare_voidptr(data) void *data __attribute__((cleanup(voidptr_raii_close)))
#define declare_fd(data) int data __attribute__((unused,cleanup(fd_raii_close)))