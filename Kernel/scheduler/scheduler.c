// Algoritmo: Round-Robin con prioridades

#include "./include/scheduler.h"

QueueNode *current;

void initScheduler()
{
    initializeQueue(&PCBqueue);    // Lista de PCBs
    initializeQueue(&round_robin); // Lista de los procesos en round-robin
}

int getCurrentPid()
{
    return current->pcb->pid;
}

int getCurrentPPid()
{
    return current->pcb->ppid;
}