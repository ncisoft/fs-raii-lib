#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef struct buffer {
    size_t capacity;
    char   *data;
} buffer_t;

extern buffer_t *buffer_new(size_t capacity);
extern void buffer_free(buffer_t *pbuf);
