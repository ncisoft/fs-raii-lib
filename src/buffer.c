#include "ipc-bus/buffer.h"
#include <ipc-bus/utils.h>
#include <stdbool.h>
#include <string.h>

#define buffer_t_sizeof sizeof(buffer_t)

buffer_t *buffer_new(size_t capacity) {
  return buffer_new_with_memory(NULL, capacity, false);
}

buffer_t *buffer_new_with_memory(void *from, size_t len,
                                   bool do_free_from_buf) {
  size_t capacity = len + 1;
  size_t total_size = capacity + buffer_t_sizeof;
  buffer_t *obj = (buffer_t *)calloc(total_size, 1);

  if (obj == NULL) {
    die(1, "can't malloc memory");
  }

  obj->capacity = capacity;
  obj->len = len;
  obj->data = cast(void *, obj + 1);

  if (from != NULL) {
    memcpy(obj->data, from, len);
    if (do_free_from_buf) {
      free(from);
    }
  }
  return obj;
}

void buffer_free(buffer_t *pbuf) {
  if (pbuf)
    free(pbuf);
}

void buffer_memcpy(buffer_t *dest_buf, const void *src, size_t n) {
  if (dest_buf == NULL)
    return;
  if (src == NULL) {
    die(1, "src == NULL");
  }
  if (n > dest_buf->capacity) {
    die(1, "n > dest_buf->capacity");
  }
  memcpy(dest_buf->data, src, n);
}

void buffer_strcpy(buffer_t *dest_buf, const char *src) {
  size_t len = strlen(src);
  return buffer_memcpy(dest_buf, src, len);
}

extern void buffer_strncpy(buffer_t *dest_buf, const void *src, size_t n) {
  buffer_memcpy(dest_buf, src, n);
}
extern buffer_t *buffer_clone(buffer_t *from_buf, bool do_free_from_buf) {
  buffer_t *new_obj = buffer_new_with_memory(from_buf->data, from_buf->len, false);
  if (from_buf != NULL && do_free_from_buf) {
    buffer_free(from_buf);
  }
  return new_obj;
}
buffer_t *buffer_strdup(const char *s) {
  size_t len = strlen(s);
  buffer_t *obj = buffer_new_with_memory((void *)s,len, false);
  return obj;
}

buffer_t *buffer_strndup(const char *s, size_t len) {
  buffer_t *obj = buffer_new_with_memory((void *)s,len, false);
  return obj;
}
