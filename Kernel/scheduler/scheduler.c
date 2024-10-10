// Algoritmo: Round-Robin con prioridades

#include "./include/scheduler.h"

void initScheduler()
{
    initializeQueue(&PCBqueue);    // Lista de PCBs
    initializeQueue(&round_robin); // Lista de los procesos en round-robin
}