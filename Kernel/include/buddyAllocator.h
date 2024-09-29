#ifndef BUDDY_ALLOCATOR_H
#define BUDDY_ALLOCATOR_H

#include <stddef.h>
#include <stdbool.h>

/* Ir ajustando a lo largo del tp */
#define MAX_LEVELS 10  // max nros de niveles que podemos manejar
#define MIN_BLOCK_SIZE 32  // Tamaño minimo de un bloque (en bytes) 
#define HEAP_START ((void *) 0xA00000)  // 10 Mb

// Estructura de un bloque de memoria
typedef struct Block {
    struct Block *next;    // Apunta al siguiente bloque en la lista de bloques libres
    bool is_free;          
    size_t size;          
} Block;

// Declaración de la lista de bloques libres organizada por niveles
extern Block* free_list[MAX_LEVELS];

// Funciones para la gestión del allocator

/**
 * Inicializa el buddy allocator con un bloque de memoria de tamaño 'size'.
 * @param ptr: Puntero al bloque de memoria.
 * @param size: Tamaño total del bloque inicial.
 */
void init_buddy_allocator(void *ptr, size_t size);

/**
 * Encuentra el nivel más pequeño que puede acomodar un bloque del tamaño 'size'.
 * @param size: Tamaño del bloque solicitado.
 * @return El nivel correspondiente.
 */
int find_level(size_t size);

/**
 * Divide un bloque en dos buddies más pequeños.
 * @param level: Nivel en el que se encuentra el bloque a dividir.
 */
void split_block(int level);

/**
 * Asigna un bloque de memoria del tamaño solicitado.
 * @param size: Tamaño del bloque solicitado.
 * @return Un puntero al bloque de memoria asignado o NULL si no se pudo asignar.
 */
void* buddy_allocate(size_t size);

/**
 * Combina buddies cuando ambos están libres.
 * @param level: Nivel en el que se encuentran los bloques a combinar.
 */
void coalesce(int level);

/**
 * Libera un bloque de memoria previamente asignado.
 * @param ptr: Puntero al bloque que se desea liberar.
 */
void buddy_free(void* ptr);

#endif // BUDDY_ALLOCATOR_H
