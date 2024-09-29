#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <buddyAllocator.h>

void* heap_start;
size_t heap_size;

// Lista de bloques libres organizada por niveles de tamaño
Block* free_list[MAX_LEVELS];

// Función para encontrar el nivel de tamaño mas chico que puede contener el tamaño size
int find_level(size_t size) {
    int level = 0;
    while ((MIN_BLOCK_SIZE << level) < size) {
        level++;
    }
    return level;
}

void init_buddy_allocator(void *ptr, size_t s) {
    heap_start = ptr;
    heap_size = s;

    for (int i = 0; i < MAX_LEVELS; i++) {
        free_list[i] = NULL;
    }
    
    Block* initial_block = (Block*) heap_start;
    initial_block->size = heap_size;
    initial_block->is_free = true;
    initial_block->next = NULL;
    
    int level = find_level(heap_size);
    free_list[level] = initial_block;
}

// Divide un bloque en dos buddies
void split_block(int level) {
    Block* block = free_list[level];
    if (block == NULL) return;
    
    free_list[level] = block->next;  // Elimina el bloque de la lista
    
    size_t new_size = block->size / 2;

    Block *buddy1 = block;
    buddy1->size = new_size;
    buddy1->is_free = true;
    
    Block* buddy2 = (Block*)((char*)block + new_size);
    buddy2->size = new_size;
    buddy2->is_free = true;
    
    // Coloca los buddies en la lista de bloques libres del nivel inferior
    buddy1->next = buddy2;
    buddy2->next = free_list[level - 1];
    free_list[level - 1] = buddy1;
}

// Reserva un bloque de memoria de tamaño size
void* buddy_allocate(size_t size) {
    int level = find_level(size);
    
    for (int i = level; i < MAX_LEVELS; i++) {
        if (free_list[i] != NULL) {
            // Encontramos un bloque en el nivel i
            while (i > level) {
                split_block(i);  // Divide los bloques hasta llegar al tamaño adecuado
                i--;
            }
            
            Block* block = free_list[level];
            free_list[level] = block->next;
            block->is_free = false;
            return (void*) block;
        }
    }
    
    // No hay bloques libres suficientemente grandes
    return NULL;
}

// Combina buddies cuando ambos están libres
void coalesce(int level) {
    Block* block = free_list[level];
    Block* buddy;
    
    while (block != NULL) {
        // Determina si el bloque está en una posición par o impar
        size_t block_size = block->size;
        uintptr_t block_address = (uintptr_t)block;  // Dirección de memoria del bloque

        if ((block_address / block_size) % 2 == 0) {
            // Si está en una posición par, el buddy está justo después
            buddy = (Block*)((char*)block + block_size);
        } else {
            // Si está en una posición impar, el buddy está justo antes
            buddy = (Block*)((char*)block - block_size);
        }

        // Verifica si el buddy está libre
        if (buddy->is_free) {
            // Combina ambos bloques
            Block* next = block->next;
            block->size *= 2;  // El tamaño se duplica al combinar buddies
            block->next = free_list[level + 1];
            free_list[level + 1] = block;
            return;
        }
        
        // Continúa con el siguiente bloque en la lista
        block = block->next;
    }
}


// Libera un bloque de memoria
void buddy_free(void* ptr) {
    Block* block = (Block*)ptr;
    block->is_free = true;
    
    int level = find_level(block->size);
    block->next = free_list[level];
    free_list[level] = block;
    
    // Intento coalescer después de liberar
    coalesce(level);
}