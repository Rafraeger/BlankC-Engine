#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    unsigned char* buffer;
    size_t capacity;
    size_t used;
} Arena;

typedef struct {
    void** slots;
    size_t slot_size;
    size_t count;
    size_t capacity;
} Pool;

Arena* arena_create(size_t capacity);
void* arena_alloc(Arena* arena, size_t size);
void* arena_alloc_aligned(Arena* arena, size_t size, size_t alignment);
void arena_reset(Arena* arena);
void arena_destroy(Arena* arena);

Pool* pool_create(size_t slot_size, size_t initial_capacity);
void* pool_alloc(Pool* pool);
void pool_free(Pool* pool, void* ptr);
void pool_destroy(Pool* pool);

#ifdef __cplusplus
}
#endif

#endif /* MEMORY_H */
