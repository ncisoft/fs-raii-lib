#include "ipc-bus/buffer.h"
#include <ipc-bus/utils.h>
#include <stdbool.h>
#include <string.h>

#define ut_buffer_t_sizeof sizeof(ut_buffer_t)

ut_buffer_t *ut_buffer_new(size_t capacity) {
  return ut_buffer_new_with_memory(NULL, capacity, false);
}

ut_buffer_t *ut_buffer_new_with_memory(void *from, size_t len,
                                       bool do_free_from_buf) {
  size_t capacity = len + 1;
  size_t total_size = capacity + ut_buffer_t_sizeof;
  ut_buffer_t *obj = (ut_buffer_t *)calloc(total_size, 1);

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

void ut_buffer_free(ut_buffer_t *pbuf) {
  if (pbuf)
    free(pbuf);
}

void ut_buffer_memcpy(ut_buffer_t *dest_buf, const void *src, size_t n) {
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

void ut_buffer_strcpy(ut_buffer_t *dest_buf, const char *src) {
  size_t len = strlen(src);
  return ut_buffer_memcpy(dest_buf, src, len);
}

extern void ut_buffer_strncpy(ut_buffer_t *dest_buf, const void *src, size_t n) {
  ut_buffer_memcpy(dest_buf, src, n);
}
extern ut_buffer_t *ut_buffer_clone(ut_buffer_t *from_buf, bool do_free_from_buf) {
  ut_buffer_t *new_obj = ut_buffer_new_with_memory(from_buf->data, from_buf->len, false);
  if (from_buf != NULL && do_free_from_buf) {
    ut_buffer_free(from_buf);
  }
  return new_obj;
}
ut_buffer_t *ut_buffer_strdup(const char *s) {
  size_t len = strlen(s);
  ut_buffer_t *obj = ut_buffer_new_with_memory((void *)s,len, false);
  return obj;
}

ut_buffer_t *ut_buffer_strndup(const char *s, size_t len) {
  ut_buffer_t *obj = ut_buffer_new_with_memory((void *)s,len, false);
  return obj;
}
