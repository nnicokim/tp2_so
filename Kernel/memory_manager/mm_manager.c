#include <stdio.h>
#include <stdint.h>
#include "./include/buddyAllocator.h"


#define MM_START ((void *)0xA00000) // 10 Mb
#define BUFFER_SIZE 100

void *start;
size_t size, currentBlock;
void *free_ptrs[BLOCK_COUNT];
size_t espacio=BLOCK_SIZE*BLOCK_COUNT;

void *mymalloc(size_t size)
{
    if (currentBlock < BLOCK_COUNT && size <= BLOCK_SIZE)
    {
        espacio -= BLOCK_SIZE;
        return free_ptrs[currentBlock++];
    }
    else
        return NULL;
}

void myfree(void *ptr)
{
    if (ptr == NULL || ptr < start)
        return;
    free_ptrs[--currentBlock] = ptr;
}

void * my_mm_init(void *ptrs, size_t s)
{
    printArray("Using List Memory Manager\n\n");
    start = ptrs;
    size = s;
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = start + i * BLOCK_SIZE;
    }
    currentBlock = 0;
    return start;
}

void mem()
{
    printArray("Memory map: \n");
    // // char buffer[BUFFER_SIZE];
    // for (int i = 0; i < BLOCK_COUNT; i++)
    // {
    //     if(free_ptrs[i] == NULL){
    //         continue;
    //     }
    //     // uintToBase((uint64_t)free_ptrs[i], buffer, 16);
    //     // printArray("Block:");
    //     // printDec(i);
    //     // printArray(buffer);
    //     // putChar('\n');
    // }
    printArray("Espacio disponible: ");
    printDec(espacio);
    printArray("\n");
}
