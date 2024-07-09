#ifndef HM_HASH_MAP
#define HM_HASH_MAP

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef uint32_t(*hash_map_hash_fn)(const void *key, size_t key_size);

typedef struct hash_map_entry {
    void *key;
    size_t key_size;
    uint32_t hash;
    void *value;
    struct hash_map_entry *next;
} hash_map_entry;

typedef struct {
    hash_map_hash_fn hash_fn;

    hash_map_entry **buckets;
    size_t bucket_count;
    size_t count;
} hash_map;

typedef void(*hash_map_iter_fn)(hash_map_entry *entry);

uint32_t hash_map_simple_hash(const void *key, size_t key_size);

hash_map *hash_map_create(size_t bucket_count);

hash_map *hash_map_create_custom_hash_fn(size_t bucket_count, hash_map_hash_fn hash_fn);

void hash_map_resize(hash_map *map, size_t new_bucket_count);

void hash_map_add(hash_map *map, const void *key, size_t key_size, void *value);

void *hash_map_get(hash_map *map, const void *key, size_t key_size);

void hash_map_remove(hash_map *map, const void *key, size_t key_size);

void hash_map_iter(hash_map *map, hash_map_iter_fn iter_fn);

void hash_map_destroy(hash_map *map);

#endif
