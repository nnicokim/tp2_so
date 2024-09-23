#include <stdio.h>
#include <stdint.h>
#include <mm_manager.h>
#define BLOCK_SIZE 10
#define BLOCK_COUNT 10
#define MM_START ((void *) 0xA00000)  // 10 Mb

void* start;
int size,current;
void* free_ptrs[BLOCK_COUNT];

void* mymalloc(size_t size) {
    if(current <BLOCK_COUNT){
        return free_ptrs[current++];
    }else
    return NULL;   
}

void myfree(void* ptr) {
    if(ptr == NULL || ptr < start ) //Podemos meter chequeos como si esta dentro del rango para ganar confianza
        return;
    free_ptrs[--current] = ptr;
}

void* my_mm_init() {
    start = MM_START;
    size = BLOCK_COUNT * BLOCK_SIZE;
    for(int i = 0;i < BLOCK_COUNT;i++){
        free_ptrs[i] = start;
        start += BLOCK_SIZE;
    }
    current = 0;
    start = MM_START;
}
