#include <stdio.h>
#include <stdint.h>
#include "./include/mm_manager.h"

#define MM_START ((void *)0xA00000) // 10 Mb

void *start;
int size, current;
void *free_ptrs[BLOCK_COUNT];

void *mymalloc(size_t size)
{
    if (current < BLOCK_COUNT && size <= BLOCK_SIZE)
    {
        return free_ptrs[current++];
    }
    else
        return NULL;
}

void myfree(void *ptr)
{
    if (ptr == NULL || ptr < start) // Podemos meter chequeos como si esta dentro del rango para ganar confianza
        return;
    free_ptrs[--current] = ptr;
}

void *my_mm_init(void *ptrs, int s)
{
    start = ptrs;
    size = s;
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = start + i * BLOCK_SIZE; // Ensure each block is distinct
    }
    current = 0;
    return start;
}
