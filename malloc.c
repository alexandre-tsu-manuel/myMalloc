#define _XOPEN_SOURCE 500
#include <unistd.h>
#include "malloc.h"

static inline
size_t word_align(size_t n)
{
  size_t size = sizeof(size_t) - 1;
  return ((n + size) & ~size);
}

void zerofill(void* ptr, size_t len)
{
  size_t* cptr = (size_t*) ptr;
  len /= sizeof(size_t);
  for (size_t i = 0; i < len; i++)
    *(cptr + i) = 0;
}

void wordcpy(void* dst, void* src, size_t len)
{
  size_t* sdst = (size_t*) dst;
  size_t* ssrc = (size_t*) src;
  len /= sizeof(size_t);
  for (size_t i = 0; i < len; i++)
    *(sdst + i) = *(ssrc + i);
}

static size_t chunk_size()
{
  static size_t s = 0;
  if (!s)
    s = word_align(sizeof(struct chunk));
  return s;
}

static
struct chunk* get_base(void)
{
  static struct chunk* base = NULL;

  if (!base)
  {
    base = sbrk(chunk_size());
    if (base == (void*)-1)
      return NULL;
    base->size = base->free = 0;
    base->next = base->prev = base->data = NULL;
  }

  return base;
}

static
int extend_heap(struct chunk* last, size_t size)
{
  struct chunk* c = sbrk(size + chunk_size());

  if (c == (void*)-1)
    return 0;
  last->next = c;
  c->size = size;
  c->free = 1;
  c->next = NULL;
  c->prev = last;
  c->data = c + 1;

  return 1;
}

static
struct chunk* find_chunk(size_t size)
{
  struct chunk* b = get_base();

  if (!b)
    return NULL;
  while (b->next)
    if (b->next->free && b->next->size >= size)
      return b;
    else
      b = b->next;

  return b;
}

static
struct chunk* get_chunk(void *p)
{
  if (!p
      || p < (void*) (get_base() + sizeof(struct chunk))
   || p > sbrk(0))
    return NULL;
  struct chunk* c = p - sizeof(struct chunk);
  if (c->data != p)
    return NULL;
  return c;
}

void* malloc(size_t s)
{
  if (!s)
    return NULL;

  struct chunk* curr = find_chunk(s);
  int ans = -1;

  if (!curr)
    return NULL;
  if (!curr->next)
    ans = extend_heap(curr, s);
  if (!ans)
    return NULL;
  curr = curr->next;
  curr->free = 0;

  return curr->data;
}

void free(void *p)
{
  struct chunk* c = get_chunk(p);

  if (!c)
    return;

  c->free = 1;
  if (c->prev && c->prev->free)
  {
    c->prev->next = c->next;
    c->prev->size += c->size + chunk_size();
    if (c->next)
      c->next->prev = c->prev;
    c = c->prev;
  }
  if (c->next && c->next->free)
  {
    c->size += c->next->size + chunk_size();
    c->next = c->next->next;
    if (c->next)
      c->next->prev = c;
  }
  if (!c->next)
  {
    c->prev->next = NULL;
    sbrk(-(c->size + chunk_size()));
  }
}

void* calloc(size_t n, size_t size)
{
  size_t len = n * size;
  void* p = malloc(len);
  if (p)
    zerofill(p, len);
  return p;
}

void* realloc(void* old, size_t size)
{
  if (!old)
    return malloc(size);

  struct chunk* c = get_chunk(old);

  if (!c)
    return NULL;
  if (size == 0)
  {
    free(old);
    return NULL;
  }
  if (size <= c->size)
    return old;
  void* p = malloc(size);
  wordcpy(p, old, size);
  free(old);

  return p;
}
