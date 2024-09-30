#ifndef COLLATZ_CACHE_H
#define COLLATZ_CACHE_H

#define UNUSED -1  // Value to mark unused cache entries

typedef struct {
  int key;    // The number being calculated
  int value;  // The number of Collatz steps for that number
  int usage;  // For LRU: last access time; for LFU: frequency of use
} CacheEntry;

// Function declarations
int collatz_steps(int n);
void initialize_cache(CacheEntry* cache, int cache_size);
int collatz_with_lru_cache(CacheEntry* cache, int cache_size, int n,
                           int* cache_hit, int* access_time);
int collatz_with_lfu_cache(CacheEntry* cache, int cache_size, int n,
                           int* cache_hit);
int find_lru_index(CacheEntry* cache, int cache_size);
int find_lfu_index(CacheEntry* cache, int cache_size);

#endif
