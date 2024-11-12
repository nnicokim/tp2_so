#include "./include/pcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <videoDriver.h>
#include "../include/naiveConsole.h"
#include "../memory_manager/include/mm_manager.h"
#include "./include/stack.h"

void initPCB(PCB *pcb, int pid, int ppid, int priority, int *fds)
{
    pcb->pid = pid;
    pcb->ppid = ppid;
    pcb->state = READY;
    pcb->name = NULL;
    pcb->priority = priority;
    pcb->priorityLife = priority;
    pcb->runningCounter = 0;

    memcpy(pcb->FD, fds, sizeof(int) * 2);

    pcb->baseAddress = NULL;
    pcb->limit = 0;
}

void freePCB(PCB *pcb)
{
    myfree(pcb);
}

int compare_PCB(const PCB *pcb1, const PCB *pcb2)
{
    return pcb1->pid - pcb2->pid;
}

void printPCB(PCB *pcb)
{
    if (pcb->pid == 0)
    {
        printArray("SHELL: \n");
        printVars(pcb);
    }
    else if (pcb->pid == 1)
    {
        printArray("IDLE: \n");
        printVars(pcb);
    }
    else if (pcb->state == FINISHED)
    {
        char *processName = pcb->name;
        printArray(processName);
        printArray(" FINISHED\n");
        return;
    }
    else
    {
        char *processName = pcb->name;
        printArray(processName);
        printArray(": \n");
        printVars(pcb);
    }
}

void printVars(PCB *pcb)
{
    printArray("PID: ");
    printDec(pcb->pid);
    printArray(" || PPID: ");
    printDec(pcb->ppid);
    printArray(" || State: ");
    printDec(pcb->state);
    printArray(" || Priority: ");
    printDec(pcb->priority);
    printArray(" || Priority Life: ");
    printDec(pcb->priorityLife);
    printArray(" || Running Counter: ");
    printDec(pcb->runningCounter);
    printArray(" || RSP: ");
    printHex((uint64_t)pcb->stack);
    printArray(" || Base Address: ");
    printHex((uint64_t)pcb->baseAddress);
    printArray("\n");
}