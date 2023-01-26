#pragma once

#include <ipc-bus/types.h>

extern char *mpool_strdup(const char *source);
extern char *mpool_strndup(const char *source, size_t n);
extern void ut_dump_hex(const char *tag, const void *data, size_t size);