#ifndef HEADER_MALLOC
  #define HEADER_MALLOC

  #include <unistd.h>

  struct chunk {
    struct chunk *next, *prev;
    size_t size;
    long free;
    void* data;
  };

  void* malloc(size_t s);
  void free(void *p);
  void* calloc(size_t n, size_t size);
  void* realloc(void* old, size_t size);

#endif
