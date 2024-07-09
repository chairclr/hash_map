#include "hash_map.h"

// Simple FNV-1a32 hash function
uint32_t hash_map_simple_hash(const void *key, size_t key_size) {
    uint32_t hash = 2166136261u;
    const uint8_t *ptr = (const uint8_t *)key;
    for (size_t i = 0; i < key_size; i++) {
        hash ^= ptr[i];
        hash *= 16777619u;
    }

    return hash;
}

// Creates a hash_map with the default hash function
hash_map *hash_map_create(size_t bucket_count) {
    hash_map *map = malloc(sizeof(hash_map));
    map->hash_fn = hash_map_simple_hash;
    map->buckets = calloc(bucket_count, sizeof(hash_map_entry *));
    map->bucket_count = bucket_count;
    map->count = 0;

    return map;
}

// Creates a hash_map with a custom hash function
hash_map *hash_map_create_custom_hash_fn(size_t bucket_count, hash_map_hash_fn hash_fn) {
    hash_map *map = malloc(sizeof(hash_map));
    map->hash_fn = hash_fn;
    map->buckets = calloc(bucket_count, sizeof(hash_map_entry *));
    map->bucket_count = bucket_count;
    map->count = 0;

    return map;
}

// Calculates the load factor of a hash_map
float hash_map_calculate_load_factor(hash_map *map) {
    return (float)map->count / (float)map->bucket_count;
}

// Resizes a hash_map to have a new number of buckets
void hash_map_resize(hash_map *map, size_t new_bucket_count) {
    hash_map_entry **old_buckets = map->buckets;
    size_t old_bucket_count = map->bucket_count;

    map->buckets = calloc(new_bucket_count, sizeof(hash_map_entry *));
    map->bucket_count = new_bucket_count;

    for (size_t i = 0; i < old_bucket_count; i++) {
        hash_map_entry *entry = old_buckets[i];
        if (!entry) {
            continue;
        }
        size_t new_index = entry->hash % map->bucket_count;
        map->buckets[new_index] = entry;
    }

    free(old_buckets);
}

// Adds or edits an entry of a hash_map specified by key
void hash_map_add(hash_map *map, const void *key, size_t key_size, void *value) {
    uint32_t hash = map->hash_fn(key, key_size);
    size_t index = hash % map->bucket_count;
    hash_map_entry *entry = map->buckets[index];
    while (entry) {
        if (entry->key_size == key_size && memcmp(entry->key, key, key_size) == 0) {
            entry->value = value;
            return;
        }
        entry = entry->next;
    }

    if (hash_map_calculate_load_factor(map) > 0.75f) {
        hash_map_resize(map, map->bucket_count * 2);
    }

    index = hash % map->bucket_count;

    hash_map_entry *new_entry = malloc(sizeof(hash_map_entry));
    new_entry->key = malloc(key_size);
    memcpy(new_entry->key, key, key_size);
    new_entry->key_size = key_size;
    new_entry->hash = hash;
    new_entry->value = value;
    new_entry->next = map->buckets[index];
    map->buckets[index] = new_entry;

    map->count++;
}

// Returns the value of a hash_map entry specified by key
void *hash_map_get(hash_map *map, const void *key, size_t key_size) {
    uint32_t hash = map->hash_fn(key, key_size);
    size_t index = hash % map->bucket_count;
    hash_map_entry *entry = map->buckets[index];
    while (entry) {
        if (entry->key_size == key_size && memcmp(entry->key, key, key_size) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }

    return NULL;
}

// Removes an entry from a hash_map by key
void hash_map_remove(hash_map *map, const void *key, size_t key_size) {
    uint32_t hash = map->hash_fn(key, key_size);
    size_t index = hash % map->bucket_count;
    hash_map_entry *entry = map->buckets[index];
    hash_map_entry *prev = NULL;
    while (entry) {
        if (entry->key_size == key_size && memcmp(entry->key, key, key_size) == 0) {
            if (prev) {
                prev->next = entry->next;
            } else {
                map->buckets[index] = entry->next;
            }
            free(entry->key);
            free(entry);
            map->count--;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}
 
// Iterates over every entry in the hash_map and calls iter_fn with it
void hash_map_iter(hash_map *map, hash_map_iter_fn iter_fn) {
    for (size_t i = 0; i < map->bucket_count; i++) {
        hash_map_entry *entry = map->buckets[i];
        while (entry) {
            iter_fn(entry);

            entry = entry->next;
        }
    }
}

// Destroys and frees the hash_map
void hash_map_destroy(hash_map *map) {
    for (size_t i = 0; i < map->bucket_count; i++) {
        hash_map_entry *entry = map->buckets[i];
        while (entry) {
            hash_map_entry *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    free(map->buckets);
    free(map);
}

