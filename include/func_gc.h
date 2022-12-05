#pragma once

#ifndef __GNUC__
#error   RAII For c  support gcc only
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <libcork/core.h>

/* Funnction Scope RAII */

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct mpool_item mpool_item_t;

struct mpool_item
  {
    mpool_item_t *m_next;
    void *p;
  };

typedef struct mempool mempool_t;
struct mempool
  {
    size_t m_capacity;
    size_t m_used;
    size_t m_counter;
    void *mp;
    mpool_item_t m_head;
  };

typedef struct fs_raii fs_raii_t;
typedef fs_raii_t *fs_raii_ptr_t;
struct fs_raii
  {
    mempool_t m_pool;
  };

#ifndef cast
#define cast(type, expr) ((type)(expr))
#endif

extern mempool_t *mp_init(mempool_t *mpool, size_t capacity);
extern void mp_destory(mempool_t *pool);
extern void *mp_malloc(mempool_t *pool, size_t size);
extern void *mp_calloc(mempool_t *pool, size_t nmemb, size_t size);
extern void *mp_realloc(mempool_t *pool, void *ptr, size_t size);
extern char *mp_strdup(mempool_t *pool, const char *s);
extern char *mp_strndup(mempool_t *pool, const char *s, size_t n);
extern void mp_free(mempool_t *pool, void *p);

extern fs_raii_t *fs_raii_new(const char *my_func_name, size_t capacity);
extern void fs_raii_close(fs_raii_ptr_t *ppfs_raii);

#define using_raii_func_scope(capacity)                                                   \
  for ( fs_raii_t                                                                 \
       proot __attribute__((cleanup (fs_raii_close))) = fs_raii_new(__FUNCTION__, capacity) \
       , proot_next = proot;                                                      \
       proot_next == proot; proot_next++)

#ifdef __cplusplus
}
#endif
