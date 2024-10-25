// Algoritmo: Round-Robin con prioridades
#include <videoDriver.h>
#include "./include/scheduler.h"
#include "../structs/include/pcb.h"
#include "../structs/include/stack.h"

extern void forceTimerTick();

Stack *stack;

CircularListNode *current = NULL;
static int processID = 2;

void initScheduler()
{
    initializeQueue(&PCBqueue);           // Lista de PCBs
    initializeCircularList(&round_robin); // Lista de los procesos en round-robin
}

// uint64_t createProcess(void (*program)(int, char **), int argc, char **argv)
uint64_t createProcess(char *program, int argc, char **argv)
{
    void *newStack = mymalloc(PAGE);
    // Stack
    if (newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process.");
        printDec(processID);
        printArray("\n");
        return -1;
    }

    PCB newPCB;
    initPCB(&newPCB, processID++, getCurrentPid(), 1); // ver que prioridad se le pasa
    addQueue(&PCBqueue, &newPCB);

    newPCB.stack = newStack;
    newPCB.baseAddress = newStack;
    newPCB.limit = PAGE;

    // Stack nuevo -> RSP = RBP
    newPCB.RSP = (uint64_t)newStack + PAGE;
    newPCB.RBP = (uint64_t)newStack + PAGE;

    // Añado al scheduler
    addCircularList(&round_robin, newPCB.pid);
    printArray("addCircularList: Process with PID: ");
    printDec(newPCB.pid);
    printArray(" created :) \n");
    initStackFrame(argc, argv, program, processID - 1);

    return newPCB.pid;
}

uint64_t killProcess(int pid)
{
    if (pid == 0 || pid == 1 || pid == 2)
    {
        return -1;
    }
    PCB *pcb = get(&PCBqueue, pid);
    if (pcb == NULL || pcb->state == FINISHED)
    {
        printArray("killProcess: ERROR: Process with PID: ");
        printDec(pid);
        printArray(" not found :( \n");
        return -1;
    }

    if (pcb->state != BLOCKED)
    {
        removeFromCircularList(&round_robin, pid);
        printArray("Se removio de la lista de round-robin en Kill y no en BlockProcess.\n");
    }
    pcb->state = FINISHED;
    removeFromQueue(&PCBqueue, pid);
    printArray("killProcess: Process with PID: ");
    printDec(pid);
    printArray(" killed\n");
    return 0; // que devuelva el codigo de exit
}

int blockProcess(int pid)
{
    if (pid == 0 || pid == 1 || pid == 2)
    {
        return -1;
    }
    PCB *pcb = get(&PCBqueue, pid);
    if (pcb->state == BLOCKED || pcb->state == FINISHED || pcb == NULL)
    {
        return -1;
    }
    pcb->state = BLOCKED;
    removeFromCircularList(&round_robin, pid);
    printArray("blockProcess: Process with PID: ");
    printDec(pid);
    printArray(" blocked\n");
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
    if (pcb->pid == 12)
    {
        printArray("unblockProcess: Process with PID: ");
        printDec(pid);
        printArray(" unblocked\n");
    }
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

uint64_t *schedule()
{
    uint64_t *rsp;
    if (current == NULL)
    {
        current = round_robin.head;
        PCB *pcb = get(&PCBqueue, current->pid);
        rsp = change_context(current->pid);
    }
    else if (current->next != NULL)
    {
        PCB *pcb = get(&PCBqueue, current->pid);
        pcb->state = READY;
        current = current->next;
        pcb = get(&PCBqueue, current->pid);
        rsp = change_context(current->pid);
    }
    else
    {
        PCB *pcb = get(&PCBqueue, current->pid);
        pcb->state = READY;
        current = round_robin.head;
        pcb = get(&PCBqueue, current->pid);
        rsp = change_context(current->pid);
    }
    return rsp;
}

// Depues vemos el tema de las prioridades !!!
uint64_t *change_context(int pid)
{ // cambiar estado del proceso - cambiar el stackframe - agregar al round-robin

    PCB *pcb = get(&PCBqueue, pid);
    pcb->state = RUNNING;
    pcb->runningCounter++;

    // cambio el stackFrame
    StackFrame *frame = (StackFrame *)(pcb->RSP);
    push(&stack, frame[0]); // revisar

    // agrego al Round-robin
    addCircularList(&round_robin, pid);

    // save_context(frame); ???

    return pcb->RSP;
}

void my_exit()
{
    PCB *pcb = get(&PCBqueue, getCurrentPid());
    if (pcb == NULL || pcb->state == FINISHED)
    {
        printArray("Could not exit process\n");
        forceTimerTick();
        return;
    }
    killProcess(pcb->pid);
    forceTimerTick();
}