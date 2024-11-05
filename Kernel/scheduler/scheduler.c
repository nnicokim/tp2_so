// Algoritmo: Round-Robin con prioridades
#include <videoDriver.h>
#include "./include/scheduler.h"

extern void forceTimerTick();

int isSchedulerActive = 0;

CircularListNode *current = NULL;
static int processID = 2;

void initScheduler()
{
    initializeCircularList(&round_robin); // Lista de los procesos en round-robin
    isSchedulerActive = 1;
}

// uint64_t createProcess(void (*program)(int, char **), int argc, char **argv)
uint64_t createProcess(char *program, int argc, char **argv)
{
    void *newStack = mymalloc(PAGE);
    // StackFrame *newStackFrame = mymalloc(sizeof(StackFrame));

    // if (newStackFrame == NULL || newStack == NULL)
    if (newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process: ");
        printDec(processID);
        printArray("\n");
        return -1;
    }

    PCB newPCB;
    initPCB(&newPCB, processID, getCurrentPid(), 1); // ver que prioridad se le pasa
    PCB_array[processID] = newPCB;

    newPCB.stack = initStackFrame(newStack, argc, argv, (void *)program, processID);
    processID++;
    newPCB.baseAddress = newStack;
    newPCB.limit = PAGE;

    newPCB.s_frame = newStack;

    addCircularList(&round_robin, newPCB.pid);
    printArray("Se creo el proceso con PID: ");
    printDec(newPCB.pid);
    printArray(" :) \n");

    return newPCB.pid;
}

void idleProcess()
{
    printArray("idleProcess: Entering idle process\n");
    while (TRUE)
    {
        _hlt();
    }
}

void createIdleProcess()
{
    void *newStack = mymalloc(PAGE);
    StackFrame *newStackFrame = mymalloc(sizeof(StackFrame));

    if (newStackFrame == NULL || newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process: ");
        printDec(processID);
        printArray("\n");
        return;
    }

    PCB PCBidle;
    initPCB(&PCBidle, IDLE_PID, SHELL_PID, 0);
    PCB_array[IDLE_PID] = PCBidle;

    PCBidle.stack = initStackFrame(newStack, 0, NULL, idleProcess, IDLE_PID);
    PCBidle.baseAddress = newStack;
    PCBidle.limit = PAGE;

    PCBidle.s_frame = newStackFrame;

    addCircularList(&round_robin, IDLE_PID);
}

uint64_t killProcess(int pid)
{
    if (pid == 0 || pid == 1 || pid == 2)
    {
        return -1;
    }
    PCB *pcb = &PCB_array[pid];
    if (pcb == NULL || pcb->state == FINISHED)
    {
        printArray("killProcess: ERROR: Process with PID: ");
        printDec(pid);
        printArray(" not found :( \n");
        return -1;
    }

    if (pcb->state != BLOCKED)
        removeFromCircularList(&round_robin, pid);

    pcb->state = FINISHED;

    // Liberamos ambos stacks
    myfree(pcb->baseAddress);
    myfree(pcb->s_frame);
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
    PCB *pcb = &PCB_array[pid];
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
    PCB *pcb = &PCB_array[pid];
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
    PCB *pcb = &PCB_array[currentPid];
    return pcb->ppid;
}

CircularListNode *getCurrentProcess()
{
    return current;
}

// void *schedule(void *rsp)
// {
//     if (current == NULL)
//     {
//         current = round_robin.head;
//         return rsp;
//     }
//     PCB *pcb = &PCB_array[current->pid];
//     pcb->state = READY;
//     pcb->stack = rsp;

//     // agrego al Round-robin asi vuelve a correr este proceso (con prioridades)
//     for (int i = 0; i <= pcb->priority; i++)
//         addCircularList(&round_robin, pcb->pid);

//     // Siguiente proceso a correr
//     current = current->next != NULL ? current->next : round_robin.head;

//     return change_context(current->pid);
// }

// void *change_context(int pid)
// {
//     PCB *pcb = &PCB_array[pid];

//     while (pcb->state == BLOCKED || pcb->state == FINISHED)
//     {
//         current = current->next != NULL ? current->next : round_robin.head;
//         pcb = &PCB_array[current->pid];
//     }
//     pcb->state = RUNNING;
//     pcb->runningCounter++;

//     return pcb->stack;
// }

void *schedule(void *rsp) // Scheduler DUMMY
{
    if (current == NULL)
    {
        current = round_robin.head;
        // printArray("Se inicializo el current en Schedule.\n");
        return rsp;
    }

    PCB *pcb = &PCB_array[current->pid];
    if (pcb->pid == 0 || pcb->pid == 1)
    {
        current = current->next != NULL ? current->next : round_robin.head;
        return rsp;
    }

    pcb->stack = rsp;                        // Guardo el RSP del proceso que va a dejar de correr
    addCircularList(&round_robin, pcb->pid); // Sin prioridades

    // Siguiente proceso a correr
    current = current->next != NULL ? current->next : round_robin.head;
    pcb = &PCB_array[current->pid];
    return pcb->stack;
}

void my_nice(uint64_t pid, uint64_t newPrio)
{
    PCB *pcb = &PCB_array[pid];
    pcb->priority = newPrio;
    printArray("my_nice: Process with PID: ");
    printDec(pid);
    printArray(" priority changed to: ");
    printDec(newPrio);
    printArray("\n");
}

int increase_priority(int pid)
{
    PCB *pcb = &PCB_array[pid];
    if (pcb->priority == MAX_PRIORITY)
        return pcb->priority;
    pcb->priority++;
    return pcb->priority;
}

int decrease_priority(int pid)
{
    PCB *pcb = &PCB_array[pid];
    if (pcb->priority == 0)
        return pcb->priority;
    pcb->priority--;
    return pcb->priority;
}

void my_exit()
{
    PCB *pcb = &PCB_array[getCurrentPid()];
    if (pcb == NULL || pcb->state == FINISHED)
    {
        printArray("Could not exit process\n");
        forceTimerTick();
        return;
    }
    killProcess(pcb->pid);
    forceTimerTick();
}