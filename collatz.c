#include "collatz.h"

// computes the number of steps in the Collatz sequence for a given number
int collatz_steps(int n) {
  int steps = 0;
  while (n != 1) {
    if (n % 2 == 0)
      n /= 2;
    else
      n = 3 * n + 1;
    steps++;
  }
  return steps;
}

// Set up the cache array
void initialize_cache(CacheEntry* cache, int cache_size) {
  for (int i = 0; i < cache_size; i++) {
    cache[i].key = UNUSED;
    cache[i].value = UNUSED;
    cache[i].usage = 0;
  }
}

// Looks for the least-recently used item in the cache
int find_lru_index(CacheEntry* cache, int cache_size) {
  int lru_index = 0;
  for (int i = 1; i < cache_size; i++) {
    if (cache[i].usage < cache[lru_index].usage) {
      lru_index = i;
    }
  }
  return lru_index;
}

// checks for the least-frequently used item in the cache
int find_lfu_index(CacheEntry* cache, int cache_size) {
  int lfu_index = 0;
  for (int i = 1; i < cache_size; i++) {
    if (cache[i].usage < cache[lfu_index].usage) {
      lfu_index = i;
    }
  }
  return lfu_index;
}

// Handles caching using the LRU method
int collatz_with_lru_cache(CacheEntry* cache, int cache_size, int n,
                           int* cache_hit, int* access_time) {
  // Look for the number in the cache
  for (int i = 0; i < cache_size; i++) {
    if (cache[i].key == n) {
      *cache_hit = 1;  // Cache hit
      cache[i].usage =
          (*access_time)++;  // Update the usage to show it's been accessed
      return cache[i].value;
    }
  }

  // Didn't find it
  *cache_hit = 0;
  int steps = collatz_steps(n);

  // Find an empty spot in the cache or replace an old entry (LRU)
  int index_to_replace = -1;
  for (int i = 0; i < cache_size; i++) {
    if (cache[i].key == UNUSED) {
      index_to_replace = i;
      break;
    }
  }

  // If no empty spot, replace the least recently used one
  if (index_to_replace == -1) {
    index_to_replace = find_lru_index(cache, cache_size);
  }

  // Put the new result in the cache
  cache[index_to_replace].key = n;
  cache[index_to_replace].value = steps;
  cache[index_to_replace].usage = (*access_time)++;  // Update for LRU

  return steps;
}

// Handles caching using the LFU method
int collatz_with_lfu_cache(CacheEntry* cache, int cache_size, int n,
                           int* cache_hit) {
  // Look for the number in the cache
  for (int i = 0; i < cache_size; i++) {
    if (cache[i].key == n) {
      *cache_hit = 1;
      cache[i].usage++;  // Increase its access count for LFU
      return cache[i].value;
    }
  }

  *cache_hit = 0;
  int steps = collatz_steps(n);

  // find an empty spot or replace the least frequently used entry
  int index_to_replace = -1;
  for (int i = 0; i < cache_size; i++) {
    if (cache[i].key == UNUSED) {
      index_to_replace = i;
      break;
    }
  }

  // if no empty spot, replace least frequently used one
  if (index_to_replace == -1) {
    index_to_replace = find_lfu_index(cache, cache_size);
  }

  // store new result in the cache
  cache[index_to_replace].key = n;
  cache[index_to_replace].value = steps;
  cache[index_to_replace].usage = 1;  // Set initial usage count

  return steps;
}
