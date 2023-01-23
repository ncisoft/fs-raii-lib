#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ipc-bus/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct buffer {
    size_t capacity;
    unsigned int len;
    void   *data;
} buffer_t;

extern buffer_t *buffer_new(size_t capacity);
extern buffer_t *buffer_new_with_memory(void *from, size_t len, bool do_free_from_buf);
extern buffer_t *buffer_strdup(const char *s);
extern buffer_t *buffer_strndup(const char *s, size_t n);
extern void buffer_free(buffer_t *pbuf);
extern void buffer_memcpy(buffer_t *dest_buf, const void *src, size_t n);
extern void buffer_strcpy(buffer_t *dest_buf, const char *src);
extern void buffer_strncpy(buffer_t *dest_buf, const void *src, size_t n);

extern buffer_t *buffer_clone(buffer_t *from_buf, bool do_free_from_buf);

#ifdef __cplusplus
}
#endif