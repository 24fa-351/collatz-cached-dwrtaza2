
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "collatz.h"

int main(int argc, char* argv[]) {
  // check for input arguments
  if (argc < 5) {
    printf("Usage: %s N MIN MAX POLICY [CACHE_SIZE]\n", argv[0]);
    return 1;
  }

  int N = atoi(argv[1]);
  int MIN = atoi(argv[2]);
  int MAX = atoi(argv[3]);
  char* policy = argv[4];  // Cache policy: "none", "LRU", or "LFU"

  // Check for valid input
  if (MIN < 1 || MAX < 1 || MIN > MAX || N < 1 ||
      (strcmp(policy, "none") != 0 && strcmp(policy, "LRU") != 0 &&
       strcmp(policy, "LFU") != 0)) {
    printf("Invalid input values.\n");
    return 1;
  }

  CacheEntry* cache = NULL;
  int cache_size = 0;
  int total_cache_hits = 0;
  int access_time = 0;  // For LRU, tracks when cache entries are accessed

  // Initialize cache if using LRU or LFU
  if (strcmp(policy, "none") != 0) {
    if (argc < 6) {
      printf("For LRU and LFU, you need to specify CACHE_SIZE.\n");
      return 1;
    }
    cache_size = atoi(argv[5]);
    if (cache_size < 1) {
      printf("Invalid CACHE_SIZE.\n");
      return 1;
    }

    // Allocate memory for the cache
    cache = (CacheEntry*)malloc(cache_size * sizeof(CacheEntry));
    if (cache == NULL) {
      printf("Memory allocation failed.\n");
      return 1;
    }

    // Initialize cache entries as unused
    initialize_cache(cache, cache_size);
  }

  srand(time(NULL));  // Random number generator seed

  // Open the CSV file for writing results
  FILE* fp = fopen("collatz_results.csv", "w");
  if (fp == NULL) {
    printf("Error opening file.\n");
    if (cache != NULL) free(cache);
    return 1;
  }

  fprintf(fp, "Random Number,Steps,Cache Policy,Cache Hit\n");

  // Process random numbers based on the given inputs
  for (int i = 0; i < N; i++) {
    int rn = rand() % (MAX - MIN + 1) +
             MIN;  // Generate a random number in the range
    int cache_hit = 0;
    int steps;

    if (strcmp(policy, "LRU") == 0) {
      steps = collatz_with_lru_cache(cache, cache_size, rn, &cache_hit,
                                     &access_time);
    } else if (strcmp(policy, "LFU") == 0) {
      steps = collatz_with_lfu_cache(cache, cache_size, rn, &cache_hit);
    } else {
      steps = collatz_steps(rn);
    }

    total_cache_hits += cache_hit;  // Count the cache hit

    // Write the result to the CSV file
    fprintf(fp, "%d,%d,%s,%s\n", rn, steps, policy, cache_hit ? "Yes" : "No");
  }

  fclose(fp); 

  // wen caching is used outputs hit statistics
  if (strcmp(policy, "none") != 0) {
    printf("Cache hit rate: %.2f%%\n", (total_cache_hits / (float)N) * 100);
  }

  printf("Results saved to collatz_results.csv\n");

  // Free the cache memory 
  if (cache != NULL) free(cache);

  return 0;
}
