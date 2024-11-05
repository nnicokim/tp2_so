#include "./include/pcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <videoDriver.h>
#include "../include/naiveConsole.h"
#include "../memory_manager/include/mm_manager.h"
#include "./include/stack.h"

void initPCB(PCB *pcb, int pid, int ppid, int priority)
{
    pcb->pid = pid;
    pcb->ppid = ppid;
    pcb->state = READY;
    pcb->priority = priority;
    pcb->runningCounter = 0;
    pcb->s_frame = NULL;

    pcb->baseAddress = NULL;
    pcb->limit = 0;
}

PCB *copyPCB(PCB *pcb, PCB *newPCB)
{
    if (!newPCB)
        newPCB = (PCB *)mymalloc(sizeof(PCB));

    newPCB->pid = pcb->pid;
    newPCB->ppid = pcb->ppid;
    newPCB->state = pcb->state;
    newPCB->priority = pcb->priority;
    newPCB->runningCounter = pcb->runningCounter;
    newPCB->s_frame = pcb->s_frame;
    newPCB->baseAddress = pcb->baseAddress;
    newPCB->limit = pcb->limit;

    return newPCB;
}

void freePCB(PCB *pcb)
{
    myfree(pcb);
}

int compare_PCB(const PCB *pcb1, const PCB *pcb2)
{
    return pcb1->pid - pcb2->pid;
}
