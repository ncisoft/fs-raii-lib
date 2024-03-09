#include "ipc-bus/types.h"
#include "ipc-bus/utils.h"
#include "logger.h"
#include <ipc-bus/fs_raii.h>
#include <ipc-bus/logger.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct 
{
  char ch;
} __attribute__ ((aligned)) align_t;

//#define nop()     ((void) 0)
extern mempool_t *mp_init(mempool_t *mpool, size_t capacity)
{
  memset(mpool, '\0', sizeof(mempool_t));
  capacity = (capacity == 0) ? UINT16_MAX : capacity;
  mpool->m_capacity = capacity;
  capacity = capacity/4*4;
  mpool->mp = calloc(1, capacity);
  mpool->mp_keep = mpool->mp;
  ut_assert(mpool->mp != NULL);
  ut_logger_warn("\t mpool->m_capacity = %d\n", cast_to_uint(capacity));
  ut_logger_debug("\t pool->mp_keep=%p\n", mpool->mp_keep);
  return mpool;
}

extern void mp_destory(mempool_t *pool)
{
  if (pool->m_capacity > 0)
    {
      mpool_item_t *head, *item;
      head = &pool->m_head;
      for (item = head->m_next; item != NULL; item = item->m_next) {
        ut_logger_context_t *ctx = &item->ctx;
        ut_logger_debug("\t release occured --> \n");
        ut_logger_debug("\t p=%p, item=%p, size=%u || m_file=%s:%s:%d\n",
                        item->p, item, cast_to_uint(item->sz), ctx->m_file,
                        ctx->m_func, ctx->m_lineno);
      }
      ut_logger_debug("\t pool->mp\t= %p\n", pool->mp);
      ut_logger_debug("\t pool->mp_keep\t= %p\n", pool->mp_keep);
    }
}

void *mp_malloc(mempool_t *pool, size_t size) 
{
  ut_assert((size + pool->m_used + sizeof(mpool_item_t)) < pool->m_capacity);
  if ((size + pool->m_used + sizeof(mpool_item_t)) > pool->m_capacity) {
    ut_assert("Insufficient pool memory" == NULL);
    exit(1);
  }

  mpool_item_t *item = cast(mpool_item_t *, pool->mp);
  char *cp = cast(char *, item + 1);
  pool->m_counter++;
  pool->m_used += size;
  pool->mp = cast(void *, cp+size);

  item->p = cp;
  item->sz = size;
  item->m_next = pool->m_head.m_next;
  pool->m_head.m_next = item;
  ut_logger_debug("\t p=%p, item=%p, sz=%d || mpool_item_t.sie=%p\n", cp, item, cast_to_int(size), sizeof_to_int(mpool_item_t));
  ut_logger_debug("\t pool->mp=%p\n", pool->mp);
  return cp;
}

extern void *mp_calloc(mempool_t *pool, size_t nmemb, size_t size) {
  void *cp = mp_malloc(pool, nmemb*size);
  memset(cp, '\0', nmemb*size);
  return cp;
}
extern void *mp_realloc(mempool_t *pool, void *ptr, size_t size)
{
 return NULL;
}
char *mp_strdup(mempool_t *pool, const char *s)
{
 size_t size = strlen(s);
 char * cp = mp_malloc(pool, size);
 strncpy(cp, s, size);
 return cp;
}

char *mp_strndup(mempool_t *pool, const char *s, size_t n)
{
 size_t size = n;
 char * cp = mp_malloc(pool, size + 1);
 strncpy(cp, s, size);
 return cp;
}

void *mp_free(mempool_t *pool, void *p)
{
  free(p);
  return NULL;
}

fs_raii_t *fs_raii_new(const char *my_func_name, size_t capacity)
{
 void *p = malloc(sizeof(fs_raii_t));
 fs_raii_t *pfs_raii = cast(fs_raii_ptr_t, p);
 mp_init(&pfs_raii->m_pool, capacity);
 return pfs_raii;
}

void fs_raii_close(fs_raii_ptr_t *ppfs_raii)
{
  fs_raii_ptr_t pfs_raii = *ppfs_raii;
  mp_destory(&pfs_raii->m_pool);
}

void *ut_fs_malloc(fs_raii_t *fsctx, size_t size, ut_logger_context_t *ctx)
{
  ut_assert(ctx != NULL && ctx->m_file != NULL);
  _ut_shorten_filename(ctx);
  void *p = mp_malloc(&fsctx->m_pool, size);
  char *cp = cast(char *, p);
  cp -= sizeof(mpool_item_t);
  mpool_item_t *item = cast(mpool_item_t *, cp);
//  ctx->m_file = strdup(ctx->m_file);
  item->ctx = *ctx;
  item->sz = size;
  ut_logger_debug("\t p=%p, item=%p, size=%d || m_file=%s:%s:%d\n", p, item, cast_to_uint(size), ctx->m_file, ctx->m_func, ctx->m_lineno);
  ut_logger_debug("\t align_t=%d\n", cast_to_uint(sizeof(align_t)));
  printf("\n");
  return p;

}
void *fs_free(fs_raii_t *fsctx, void *ptr)
{
  void *p = mp_free(&fsctx->m_pool, ptr);
 return p;
}