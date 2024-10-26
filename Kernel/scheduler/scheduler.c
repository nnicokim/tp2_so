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
    newPCB.RSP = (uint64_t *)newStack + PAGE;
    newPCB.RBP = (uint64_t *)newStack + PAGE;

    // Añado al scheduler
    addCircularList(&round_robin, newPCB.pid);
    printArray("addCircularList: Process with PID: ");
    printDec(newPCB.pid);
    printArray(" created :) \n");
    initStackFrame(argc, argv, (void *)program, processID - 1);

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

uint64_t *schedule() // void
{
    uint64_t *rsp;
    if (current == NULL)
    {
        current = round_robin.head;
        // PCB *pcb = get(&PCBqueue, current->pid);
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
    // save_context(frame); ???

    StackFrame *frame = (StackFrame *)(pcb->RSP);
    push(stack, frame[0]); // revisar
    // sacar del stack el contexto del proceso a correr

    // agrego al Round-robin
    if (pcb->state != BLOCKED || pcb->priority == 0)
        addCircularList(&round_robin, pid);
    else if (pcb->state != BLOCKED)
    {
        for (int i = 0; i < pcb->priority; i++)
            addCircularList(&round_robin, pid);
    }

    return pcb->RSP;
}

void my_nice(uint64_t pid, uint64_t newPrio)
{
    PCB *pcb = get(&PCBqueue, pid);
    pcb->priority = newPrio;
    printArray("my_nice: Process with PID: ");
    printDec(pid);
    printArray(" priority changed to: ");
    printDec(newPrio);
    printArray("\n");
}

int increase_priority(int pid)
{
    PCB *pcb = get(&PCBqueue, pid);
    pcb->priority++;
    return pcb->priority;
}

int decrease_priority(int pid)
{
    PCB *pcb = get(&PCBqueue, pid);
    if (pcb->priority == 0)
        return pcb->priority;
    pcb->priority--;
    return pcb->priority;
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