// Algoritmo: Round-Robin con prioridades
#include <videoDriver.h>
#include "./include/scheduler.h"
#include "../structs/include/pcb.h"
#include "../structs/include/stack.h"

#define PAGE 0x1000

extern void forceTimerTick();

int isSchedulerActive = 0;
Stack *stack;

CircularListNode *current = NULL;
static int processID = 2;

void initScheduler()
{
    initializeQueue(&PCBqueue);           // Lista de PCBs
    initializeCircularList(&round_robin); // Lista de los procesos en round-robin
    isSchedulerActive = 1;
}

// uint64_t createProcess(void (*program)(int, char **), int argc, char **argv)
uint64_t createProcess(char *program, int argc, char **argv)
{
    StackFrame *newStackFrame = mymalloc(sizeof(StackFrame));
    void *newStack = mymalloc(PAGE);
    // StackFrame
    if (newStackFrame == NULL || newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process.");
        printDec(processID);
        printArray("\n");
        return -1;
    }

    PCB newPCB;
    initPCB(&newPCB, processID++, getCurrentPid(), 1); // ver que prioridad se le pasa
    addQueue(&PCBqueue, &newPCB);

    // Stack nuevo -> RSP = RBP
    newPCB.stack = initStackFrame(newStack, argc, argv, (void *)program, processID - 1);
    newPCB.baseAddress = newPCB.stack;
    newPCB.limit = PAGE;

    newPCB.s_frame = newStackFrame;

    // Añado al scheduler
    addCircularList(&round_robin, newPCB.pid);
    printArray("addCircularList: Process with PID: ");
    printDec(newPCB.pid);
    printArray(" created :) \n");

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

void sleepCurrent()
{
    if (current == NULL)
    {
        current = round_robin.head;
        return;
    }
    PCB *pcb = get(&PCBqueue, current->pid);
    pcb->state = READY;
    // save_current_context(pcb->s_frame);
    current = current->next != NULL ? current->next : round_robin.head;
}

// Debemos devolver el RSP del proceso que comienza a correr
void *schedule() // void *
{
    sleepCurrent();
    printArray("Cambiando contexto... \n");
    return change_context(current->pid);
}

void *change_context(int pid)
{ // cambiar estado del proceso - cambiar el stackframe - agregar al round-robin

    PCB *pcb = get(&PCBqueue, pid);
    while (pcb->state == BLOCKED)
    { // Me salteo todos los procesos bloqueados hasta llegar al proximo proceso READY
        current = current->next != NULL ? current->next : round_robin.head;
    }
    pcb->state = RUNNING;
    pcb->runningCounter++;

    // Agarro el stack frame del proceso.
    // StackFrame *frame = pcb->s_frame;
    // load_current_context(frame);

    // agrego al Round-robin (con prioridades)
    if (pcb->state != BLOCKED || pcb->priority == 0)
        addCircularList(&round_robin, pid);
    else if (pcb->state != BLOCKED)
    {
        for (int i = 0; i < pcb->priority; i++)
            addCircularList(&round_robin, pid);
    }

    printArray("Contexto cambiado! \n");
    return pcb->stack;
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