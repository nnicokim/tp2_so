#include "./include/pcb.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <videoDriver.h>
#include "../include/naiveConsole.h"
#include "../memory_manager/include/mm_manager.h"
#include "./include/stack.h"
#include "./include/queue.h"

void initPCB(PCB *pcb, int pid, int ppid, int priority)
{
    pcb->pid = pid;
    pcb->ppid = ppid;
    pcb->state = READY;
    pcb->priority = priority;
    pcb->RSP = 0;
    pcb->RBP = 0;

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
    newPCB->RSP = pcb->RSP;
    newPCB->RBP = pcb->RBP;
    newPCB->baseAddress = pcb->baseAddress;
    newPCB->limit = pcb->limit;

    return newPCB;
}

void freePCB(PCB *pcb)
{
    myfree(pcb);
}

// void printPCB(PCB *pcb)
// {
//     ncPrint("PID: %d\n", pcb->pid);
//     ncPrint("PPID: %d\n", pcb->ppid);
//     ncPrint("State: %d\n", pcb->state);
//     ncPrint("Priority: %d\n", pcb->priority);
//     ncPrint("RSP: %d\n", pcb->RSP);
//     ncPrint("RBP: %d\n", pcb->RBP);

//     ncPrint("Base Address: %p\n", pcb->baseAddress);
//     ncPrint("Limit: %d\n", pcb->limit);
// }

int compare_PCB(const PCB *pcb1, const PCB *pcb2)
{
    return pcb1->pid - pcb2->pid;
}

void switchToProcess(int processID) {
    PCB *pcb = get(&PCBqueue, processID);
    if (pcb == NULL) {
        printArray("switchToProcess: ERROR: Process with PID: ");
        printDec(processID);
        printArray(" not found\n");
        return;
    }
    load_context(pcb->stack);

}