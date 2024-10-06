#ifndef _MM_MANAGER_H_
#define _MM_MANAGER_H_

#include <stdint.h>
#include <stdlib.h>

#define HEAP_START ((void *) 0xA00000)  // 10 Mb


/**
  Sets up the memory manager
*/
void* my_mm_init(void* ptrs, int size);

/**
 * @brief   Allocates size bytes
 * 
 * @param   size Size of memory block to allocate
 * 
 * @return  Pointer to the allocated block of memory
 *          If NULL, then error
*/
void* mymalloc(size_t size);

/**
 * @brief   Frees a block of memory
 * 
 * @param   ptr Pointer to the memory block to be freed
*/
void myfree(void * ptr);

#endif