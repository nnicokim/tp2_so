// Algoritmo: Round-Robin con prioridades

#include "./include/scheduler.h"

CircularListNode *current;

void initScheduler()
{
    initializeQueue(&PCBqueue);           // Lista de PCBs
    initializeCircularList(&round_robin); // Lista de los procesos en round-robin
}

int blockProcess(int pid)
{
    PCB *pcb = get(&PCBqueue, pid);
    if (pcb->state == BLOCKED || pcb->state == FINISHED)
    {
        return -1;
    }
    pcb->state = BLOCKED;
    removeCircularList(&round_robin, pid);
    return 1;
}

int unblockProcess(int pid)
{
    PCB *pcb = get(&PCBqueue, pid);
    if (pcb->state != BLOCKED)
    {
        return -1;
    }
    pcb->state = READY;
    addCircularList(&round_robin, pid);
    return 1;
}

int getCurrentPid()
{
    return current->pid;
}

int getCurrentPPid()
{
    int currentPid = getCurrentPid();
    PCB *pcb = get(&PCBqueue, currentPid);
    return pcb->ppid;
}

CircularListNode *getCurrentProcess()
{
    return current;
}