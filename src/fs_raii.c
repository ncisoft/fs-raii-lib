#include <func_gc.h>
#include <assert.h>

#define nop()     ((void) 0)
extern mempool_t *mp_init(mempool_t *mpool, size_t capacity)
{
  memset(mpool, '\0', sizeof(mempool_t));
  mpool->m_capacity = capacity;
  mpool->mp = calloc(1, capacity);
  ut_assert(mpool->mp != NULL);
  return mpool;
}

extern void mp_destory(mempool_t *pool)
{
  if (pool->m_capacity > 0)
    {
      free(pool->mp);
    }
}
void *mp_malloc(mempool_t *pool, size_t size)
{
  if ((size+pool->m_used+sizeof(mpool_item_t)) < pool->m_capacity)
    {
      ut_assert("Insufficient pool memory" == NULL);
      exit(1);
    }
  mpool_item_t *item = cast(mpool_item_t *, pool->mp);
  char *cp = cast(char *, item+1);
  cp += sizeof(mpool_item_t);
  pool->m_counter++;
  pool->m_used += size;
  pool->mp = cast(void *, cp);
 item->p = cp;
  item->m_next = pool->m_head.m_next;
  pool->m_head.m_next = item;
  return cp;
}

extern void *mp_calloc(mempool_t *pool, size_t nmemb, size_t size)
{
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

void mp_free(mempool_t *pool, void *p)
{
  nop();
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
