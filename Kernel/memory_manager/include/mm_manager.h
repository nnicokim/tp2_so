#ifndef _MM_MANAGER_H_
#define _MM_MANAGER_H_

#include <stdint.h>
#include <stdlib.h>
#include "../include/videoDriver.h"

#define HEAP_START ((void *)0xA00000) // 10 Mb
#define BLOCK_SIZE 10000          
#define BLOCK_COUNT 10000

/**
  Sets up the memory manager
*/
void *my_mm_init(void *ptrs, size_t size);

/**
 * @brief   Allocates size bytes
 *
 * @param   size Size of memory block to allocate
 *
 * @return  Pointer to the allocated block of memory
 *          If NULL, then error
 */
void *mymalloc(size_t size);

/**
 * @brief   Frees a block of memory
 *
 * @param   ptr Pointer to the memory block to be freed
 */
void myfree(void *ptr);

/**
  * @brief  Prints the memory map
 */
void mem();

#endif