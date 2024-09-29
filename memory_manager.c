#include <stdio.h>
#include <stdint.h>
#define BLOCK_SIZE 50
#define BLOCK_COUNT 50

void *start;
int size, current;
void *free_ptrs[BLOCK_COUNT];

void *mymalloc(size_t size)
{
    if (current < BLOCK_COUNT)
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

void *my_mm_init(void *ptr, int s)
{
    start = ptr;
    size = s;
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        free_ptrs[i] = start;
        start += BLOCK_SIZE;
    }
    current = 0;
    start = ptr;
}
int main()
{
    char memory[BLOCK_COUNT * BLOCK_SIZE];

    // Initialize your memory manager
    my_mm_init((void *)memory, BLOCK_COUNT * BLOCK_SIZE);

    return 0;
}

// Esto es para meterlo en el tp
/*A partir de cierta direccion que le podemos llamar hay que averiguar:
#define START_FREE_MM cierta direccion
Adentro del "main": void* mem= START_FREE_MM;
Lo llamo al test dentro de la funcion
*/
