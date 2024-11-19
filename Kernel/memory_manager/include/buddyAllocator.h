#ifndef __BUDDY_ALLOCATOR_H
#define __BUDDY_ALLOCATOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../../include/videoDriver.h"

#define HEAP_START ((void *)0xA00000) // 10 Mb
#define LIST_SPACE (1 << 27)  // 128 MB

#define HEAP_START ((void *)0xA00000) // 10 Mb
#define BLOCK_SIZE 10000          
#define BLOCK_COUNT 10000

struct node {
	void * address;
	uint64_t size;
	bool is_free;
	struct node * prev;
	struct node * next;
};

typedef struct node * NodePtr;

typedef struct memoryData {
	uint32_t free, used, total;
} memoryData;

typedef memoryData * MemoryDataPtr;

void * my_mm_init(void *ptr, size_t size);

void * mymalloc(uint64_t size);

void myfree(void *ptr);

MemoryDataPtr getMemoryData();

#endif