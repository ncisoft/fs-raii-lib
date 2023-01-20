#include <stdio.h>
#include <buffer.h>
#include <utils.h>

buffer_t *buffer_new(size_t capacity)
{
  buffer_t *pbuf = malloc(capacity + sizeof(buffer_t));

  return pbuf;
}

void buffer_free(buffer_t *pbuf)
{
  free(pbuf);
}

