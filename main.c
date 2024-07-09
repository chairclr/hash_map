#include <stdlib.h>
#include <stdio.h>
#include "hash_map.h"

void iter(hash_map_entry *entry) {
    printf("Value: %s\n", (char*)entry->value);

    // Clean up values alloc'd in main
    free(entry->value);
}

int main() {
    hash_map *map = hash_map_create(32);

    size_t test_entry_count = 10;
    printf("Adding %zu entry to map\n", test_entry_count);
    for (int i = 0; i < test_entry_count; i++) {
        char* buffer = malloc(16);
        snprintf(buffer, 16, "%d", i);

        // Use i as the key
        hash_map_add(map, &i, sizeof(i), buffer);
    }

    printf("Iterating over map\n");
    hash_map_iter(map, &iter);

    hash_map_destroy(map);
    return 0;
}

