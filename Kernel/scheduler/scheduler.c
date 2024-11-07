// Algoritmo: Round-Robin con prioridades
#include <videoDriver.h>
#include "./include/scheduler.h"

extern void forceTimerTick();

int isSchedulerActive = 0;

int IDLE_counter = 0;

CircularListNode *current = NULL;
static int processID = 0;

void initScheduler()
{
    initializeCircularList(&round_robin);

    int shellPID = createProcess(_setUser, 0, NULL);

    int pidIdle = createProcess(idleProcess, 0, NULL); // Creamos el proceso (Idle)

    // current = round_robin.head; // TODO: ver como inicializarlo con el primer proceso (que no sea NULL)
    isSchedulerActive = 1;
}

// uint64_t createProcess(void (*program)(int, char **), int argc, char **argv)
uint64_t createProcess(char *program, int argc, char **argv)
{
    void *newStack = mymalloc(PAGE);
    if (newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process: ");
        printDec(processID);
        printArray("\n");
        return -1;
    }

    PCB *newPCB = mymalloc(sizeof(PCB));
    if (processID == 1 || processID == 0)
    {
        newPCB->ppid = 0;
    }
    else
    {
        newPCB->ppid = getCurrentPid();
    }
    initPCB(newPCB, processID, newPCB->ppid, 1); // ver que prioridad se le pasa por Default
    PCB_array[processID] = newPCB;

    newPCB->stack = initStackFrame(newStack + PAGE - sizeof(char), argc, argv, (void *)program, processID);
    processID++;
    newPCB->baseAddress = newStack + PAGE - sizeof(char);
    newPCB->limit = PAGE;

    addCircularList(&round_robin, newPCB->pid);
    return newPCB->pid;
}

void idleProcess()
{
    printArray("idleProcess: Entering HALT... \n");
    while (TRUE)
    {
        _hlt();
    }
}

uint64_t killProcess(int pid)
{
    if (pid == 0)
    {
        return -1;
    }
    PCB *pcb = PCB_array[pid];
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

    // Liberamos stack y pcb
    myfree(pcb->baseAddress - PAGE + sizeof(char));
    myfree(pcb);
    printArray("killProcess: Process with PID: ");
    printDec(pid);
    printArray(" killed\n");
    return 0; // que devuelva el codigo de exit
}

int blockProcess(int pid)
{
    if (pid == 0)
    {
        return -1;
    }
    PCB *pcb = PCB_array[pid];
    if (pcb->state == BLOCKED || pcb->state == FINISHED || pcb == NULL)
    {
        return -1;
    }
    pcb->state = BLOCKED;
    // removeFromCircularList(&round_robin, pid); // TODO: ver si hace falta o no sacarlo del round_robin
    printArray("blockProcess: Process with PID: ");
    printDec(pid);
    printArray(" blocked\n");
    return 1;
}

int unblockProcess(int pid)
{
    PCB *pcb = PCB_array[pid];
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
    PCB *pcb = PCB_array[currentPid];
    return pcb->ppid;
}

CircularListNode *getCurrentProcess()
{
    return current;
}

// void *schedule(void *rsp)
// {
//     PCB *pcb = PCB_array[current->pid];
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
//     PCB *pcb = PCB_array[pid];

//     while (pcb->state == BLOCKED || pcb->state == FINISHED)
//     {
//         current = current->next != NULL ? current->next : round_robin.head;
//         pcb = PCB_array[current->pid];
//     }
//     pcb->state = RUNNING;
//     pcb->runningCounter++;

//     return pcb->stack;
// }

void *schedule(void *rsp) // RSP del Proceso A
{
    if (current == NULL)
    {
        current = round_robin.head;
        PCB *pcb = PCB_array[IDLE_PID];
        return pcb->stack; // RSP del IDLE
    }

    PCB *pcb = PCB_array[current->pid];
    StackFrame *stack = rsp;
    pcb->stack = rsp; // Guardo el RSP del proceso que va a dejar de correr
    // addCircularList(&round_robin, pcb->pid); // Sin prioridades

    // Siguiente proceso a correr
    current = current->next;
    // printDec(current->pid);
    // printArray("\n");

    pcb = PCB_array[current->pid];
    while (pcb->state == BLOCKED || pcb->state == FINISHED)
    {
        current = current->next;
        pcb = PCB_array[current->pid];
    }

    pcb->runningCounter++;

    // if (pcb->pid == 2)
    // {
    //     printArray("--- CHANGED Context del PID: ---\n");
    //     printDec(pcb->pid);
    //     printArray("\n");
    //     printHex(pcb->stack->rip);
    //     printArray("\n");
    //     printHex(pcb->stack);
    //     printArray("\n");
    // }

    return pcb->stack; // RSP del proceso B
}

void my_nice(uint64_t pid, uint64_t newPrio)
{
    PCB *pcb = PCB_array[pid];
    pcb->priority = newPrio;
    printArray("my_nice: Process with PID: ");
    printDec(pid);
    printArray(" priority changed to: ");
    printDec(newPrio);
    printArray("\n");
}

int increase_priority(int pid)
{
    PCB *pcb = PCB_array[pid];
    if (pcb->priority == MAX_PRIORITY)
        return pcb->priority;
    pcb->priority++;
    return pcb->priority;
}

int decrease_priority(int pid)
{
    PCB *pcb = PCB_array[pid];
    if (pcb->priority == 0)
        return pcb->priority;
    pcb->priority--;
    return pcb->priority;
}

void my_exit()
{
    PCB *pcb = PCB_array[getCurrentPid()];
    if (pcb == NULL || pcb->state == FINISHED)
    {
        printArray("Could not exit process\n");
        forceTimerTick();
        return;
    }
    killProcess(pcb->pid);
    forceTimerTick();
}

void print_processes()
{
    printArray("Printing processes...\n");
    for (int i = 0; i < processID; i++)
    {
        if (PCB_array[i] != NULL)
        {
            printPCB(PCB_array[i]);
        }
    }
}