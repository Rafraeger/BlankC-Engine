#include "core/memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Arena* arena_create(size_t capacity) {
    Arena* arena = malloc(sizeof(Arena));
    if (!arena) return NULL;
    arena->buffer = malloc(capacity);
    if (!arena->buffer) { free(arena); return NULL; }
    arena->capacity = capacity;
    arena->used = 0;
    return arena;
}

void* arena_alloc(Arena* arena, size_t size) {
    if (arena->used + size > arena->capacity) return NULL;
    void* ptr = arena->buffer + arena->used;
    arena->used += size;
    return ptr;
}

void* arena_alloc_aligned(Arena* arena, size_t size, size_t alignment) {
    size_t offset = arena->used;
    size_t aligned = (offset + alignment - 1) & ~(alignment - 1);
    if (aligned + size > arena->capacity) return NULL;
    arena->used = aligned + size;
    return arena->buffer + aligned;
}

void arena_reset(Arena* arena) {
    arena->used = 0;
}

void arena_destroy(Arena* arena) {
    if (arena) {
        free(arena->buffer);
        free(arena);
    }
}

Pool* pool_create(size_t slot_size, size_t initial_capacity) {
    Pool* pool = malloc(sizeof(Pool));
    if (!pool) return NULL;
    pool->slot_size = slot_size;
    pool->capacity = initial_capacity;
    pool->count = 0;
    pool->slots = calloc(initial_capacity, sizeof(void*));
    if (!pool->slots) { free(pool); return NULL; }
    for (size_t i = 0; i < initial_capacity; i++) {
        pool->slots[i] = malloc(slot_size);
    }
    return pool;
}

void* pool_alloc(Pool* pool) {
    for (size_t i = 0; i < pool->capacity; i++) {
        if (pool->slots[i] != NULL) {
            void* ptr = pool->slots[i];
            pool->slots[i] = NULL;
            pool->count++;
            return ptr;
        }
    }
    return NULL;
}

void pool_free(Pool* pool, void* ptr) {
    for (size_t i = 0; i < pool->capacity; i++) {
        if (pool->slots[i] == NULL) {
            pool->slots[i] = ptr;
            pool->count--;
            return;
        }
    }
    /* Pool is full — no available slot to return ptr to.
       This is a bug in the caller: more frees than allocs, or pool is exhausted. */
    fprintf(stderr, "[WARN] pool_free: no available slot, pointer leaked\n");
}

/* Note: pool_destroy only frees slots that are currently available (non-NULL).
   Slots that were allocated via pool_alloc (set to NULL) are the caller's
   responsibility to free before calling pool_destroy. */
void pool_destroy(Pool* pool) {
    if (pool) {
        for (size_t i = 0; i < pool->capacity; i++) {
            free(pool->slots[i]);
        }
        free(pool->slots);
        free(pool);
    }
}
