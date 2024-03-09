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
} ut_buffer_t;

extern ut_buffer_t *ut_buffer_new(size_t capacity);
extern ut_buffer_t *ut_buffer_new_with_memory(void *from, size_t len, bool do_free_from_buf);
extern ut_buffer_t *ut_buffer_strdup(const char *s);
extern ut_buffer_t *ut_buffer_strndup(const char *s, size_t n);
extern void ut_buffer_free(ut_buffer_t *pbuf);
extern void ut_buffer_memcpy(ut_buffer_t *dest_buf, const void *src, size_t n);
extern void ut_buffer_strcpy(ut_buffer_t *dest_buf, const char *src);
extern void ut_buffer_strncpy(ut_buffer_t *dest_buf, const void *src, size_t n);

extern ut_buffer_t *ut_buffer_clone(ut_buffer_t *from_buf, bool do_free_from_buf);

#ifdef __cplusplus
}
#endif