#include <stdio.h>
#include <stdint.h>
#include "./include/mm_manager.h"

#define MM_START ((void *)0xA00000) // 10 Mb

void *start;
int size, currentBlock;
void *free_ptrs[BLOCK_COUNT];

void *mymalloc(size_t size)
{
    if (currentBlock < BLOCK_COUNT && size <= BLOCK_SIZE)
    {
        return free_ptrs[currentBlock++];
    }
    else
        return NULL;
}

void myfree(void *ptr)
{
    // Podemos meter chequeos como si esta dentro del rango para ganar confianza
    if (ptr == NULL || ptr < start)
        return;
    free_ptrs[--currentBlock] = ptr;
}

void *my_mm_init(void *ptrs, int s)
{
    start = ptrs;
    size = s;
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = start + i * BLOCK_SIZE; // Ensure each block is distinct
    }
    currentBlock = 0;
    return start;
}
