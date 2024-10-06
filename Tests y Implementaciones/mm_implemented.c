#include <stdio.h>
#include "test_util.h"  // Include the test_mm file
#include <stdint.h>
#define BLOCK_SIZE 100
#define BLOCK_COUNT 10
extern void test_mm(int argc, char *argv[]);  // Declare the function prototype

void* start;
int size, current;
void* free_ptrs[BLOCK_COUNT];

void* mymalloc(size_t size) {
    if (current < BLOCK_COUNT && size < BLOCK_SIZE) {
        return free_ptrs[current++];
    } else {
        return NULL;
    }
}

void myfree(void* ptr) {
    if (ptr == NULL || ptr < start) // Add checks to ensure the pointer is within range
        return;
    free_ptrs[--current] = ptr;
}

void* my_mm_init(void* memory, int s) {
    start = memory;
    size = s;
    for (int i = 0; i < BLOCK_COUNT; i++) {
        free_ptrs[i] = (char*) start + i * BLOCK_SIZE;  // Ensure each block is distinct
    }
    current = 0;

    return memory;
}

int main() {
    char memory[BLOCK_COUNT * BLOCK_SIZE];  // Memory block
    // Initialize your memory manager
    my_mm_init((void*)memory, BLOCK_COUNT * BLOCK_SIZE);
    
    // Call the test_mm function with a sample argument (e.g., "100")
    char *argv[] = {"1000"};
    test_mm(1, argv);  // Call the test function

    return 0;
}