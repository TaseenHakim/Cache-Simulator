//Taseen Hakim
//cs4541
//taseenbin.hakim@wmich.edu

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LENGTH 1024

typedef unsigned long long int mem_addr_t;

typedef struct {
    char valid; 
    mem_addr_t tag; 
    unsigned long long int timestamp; // Timestamp for FIFO policy
} cache_line_t;

typedef cache_line_t* cache_set_t;
typedef cache_set_t* cache_t;

int s, E, b, num_sets, block_size;
cache_t cache;

int hits = 0;
int misses = 0;
int evictions = 0;
unsigned long long int current_time = 0;

// Initializes the cache with given set and block parameters.
void initializeCache() {
    num_sets = (1 << s);
    block_size = (1 << b);

    cache = (cache_set_t*)malloc(num_sets * sizeof(cache_set_t));
    for (int i = 0; i < num_sets; i++) {
        cache[i] = (cache_line_t*)malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].timestamp = 0;
        }
    }
}

// Access the cache with a memory address.
void accessCache(mem_addr_t addr) {
    mem_addr_t set_idx = (addr >> b) & ((1 << s) - 1);
    mem_addr_t tag = addr >> (s + b);

    int empty_line = -1; // To keep track of an empty line in the cache set (if any).
    int replace_line = -1; // To keep track of which line to replace.
    unsigned long long int oldest_time = ULLONG_MAX; // To find the oldest line for FIFO.

    for (int i = 0; i < E; i++) {
        if (cache[set_idx][i].valid) {
            if (cache[set_idx][i].tag == tag) {
                hits++;
                return;
            }
            if (cache[set_idx][i].timestamp < oldest_time) {
                oldest_time = cache[set_idx][i].timestamp;
                replace_line = i;
            }
        } else if (empty_line == -1) {
            empty_line = i;
        }
    }

    misses++;
    if (empty_line != -1) {
        cache[set_idx][empty_line].valid = 1;
        cache[set_idx][empty_line].tag = tag;
        cache[set_idx][empty_line].timestamp = current_time++;
    } else {
        if (cache[set_idx][replace_line].valid) {
            evictions++;
        }
        cache[set_idx][replace_line].valid = 1;
        cache[set_idx][replace_line].tag = tag;
        cache[set_idx][replace_line].timestamp = current_time++;
    }
}

// Frees the memory allocated for the cache.
void freeCache() {
    for (int i = 0; i < num_sets; i++) {
        free(cache[i]);
    }
    free(cache);
}

// Prints the summary of cache hits, misses, and evictions.
void printSummary() {
    printf("hits:%d misses:%d evictions:%d\n", hits, misses, evictions);
}

// Simulates the cache based on the memory accesses from the trace file.
void simulateCache(const char *trace_file) {
    char buf[MAX_LINE_LENGTH];
    mem_addr_t addr;

    FILE *fp = fopen(trace_file, "r");
    if (!fp) {
        perror("Error opening trace file.");
        exit(EXIT_FAILURE);
    }

    while (fgets(buf, MAX_LINE_LENGTH, fp)) {
        if (buf[0] != ' ') continue;

        sscanf(buf + 1, "%*c %llx", &addr);
        accessCache(addr);
    }

    fclose(fp);
}

// Main function 
int main(int argc, char *argv[]) {
    char *trace_file = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            s = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-E") == 0) {
            E = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0) {
            b = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0) {
            trace_file = argv[++i];
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("Usage: %s -s <s> -E <E> -b <b> -t <tracefile>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    initializeCache();
    simulateCache(trace_file);
    printSummary();
    freeCache();

    return 0;
}
