// Algoritmo: Round-Robin con prioridades
#include <videoDriver.h>
#include "./include/scheduler.h"
#include "../include/time.h"

extern void forceTimerTick();

int isSchedulerActive = 0;

CircularListNode *current = NULL;
static int processID = 0;

void initScheduler()
{
    initializeCircularList(&round_robin);

    int shellFD[] = {0, 1};
    int idleFD[] = {0, 1};
    createProcess("SHELL", (void *)_setUser, 0, NULL, shellFD);
    createProcess("IDLE", (void *)idleProcess, 0, NULL, idleFD);

    isSchedulerActive = 1;
}

uint64_t createProcess(char *pr_name, void *program, int argc, char **argv, int *fds)
{
    void *newStack = mymalloc(PAGE);
    if (newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process: ");
        printDec(processID);
        printArray("\n");
        return -1;
    }
    if (processID >= MAX_PROCESSES)
    {
        printArray("createProcess: ERROR creating process. Max number of processes reached.\n");
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
    initPCB(newPCB, processID, newPCB->ppid, DEFAULT_PRIORITY, (int *)fds);
    PCB_array[processID] = newPCB;

    newPCB->stack = initStackFrame(newStack + PAGE - sizeof(char), argc, argv, (void *)program, processID);
    processID++;
    newPCB->name = pr_name;
    newPCB->baseAddress = newStack + PAGE - sizeof(char);
    newPCB->limit = PAGE;

    addCircularList(&round_robin, newPCB->pid);
    return newPCB->pid;
}

void randomFunction()
{
    printArray("Random function executed!!! \n");

    while (TRUE)
        ;
}

uint64_t createOneProcess()
{
    int dummyFD[] = {0, 1};
    return createProcess("DUMMY Function", (void *)randomFunction, 0, NULL, dummyFD);
}

void idleProcess()
{
    while (TRUE)
    {
        _hlt();
    }
}

uint64_t killProcess(int pid)
{
    if (pid == 0 || pid == 1)
    {
        return -1;
    }
    PCB *pcb = PCB_array[pid];
    if (pcb == NULL)
    {
        return -1;
    }

    if (pcb->state != FINISHED) // Esto es por el kill de la shell
    {
        pcb->state = FINISHED;
    }

    // Liberamos stack y pcb
    removeFromCircularList(&round_robin, pid);
    myfree(pcb->baseAddress - PAGE + sizeof(char)); // Libera el stack
    myfree(pcb);
    PCB_array[pid] = NULL;
    return 0; // que devuelva el codigo de exit
}

int blockProcess(int pid)
{
    PCB *pcb = PCB_array[pid];
    if (pcb->state == BLOCKED || pcb->state == FINISHED || pcb == NULL)
    {
        printArray("ERROR: Could not block process\n");
        return -1;
    }
    pcb->state = BLOCKED;
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

void *schedule(void *rsp)
{
    if (current == NULL)
    {
        current = round_robin.head;
        PCB *pcb = PCB_array[IDLE_PID];
        pcb->state = RUNNING;
        return pcb->stack;
    }

    PCB *pcb = PCB_array[current->pid];
    pcb->stack = rsp;
    if (pcb->state == RUNNING && pcb->priority > pcb->priorityLife)
    {
        pcb->priorityLife--;
        return pcb->stack;
    }

    if (pcb->state == FINISHED)
    {
        current = current->next;
        killProcess(pcb->pid);
        return change_context(current->pid);
    }
    if (pcb->state == BLOCKED)
    { // Esto es para que no se quede en un proceso bloqueado y tampoco lo desbloqueo
        current = current->next;
        return change_context(current->pid);
    }

    pcb->state = READY;
    current = current->next;

    return change_context(current->pid);
}

void *change_context(int pid)
{
    PCB *pcb = PCB_array[pid];

    while (pcb->state == BLOCKED || pcb->state == FINISHED)
    {
        if (pcb->state == FINISHED)
            killProcess(pcb->pid);

        current = current->next;
        pcb = PCB_array[current->pid];
    }
    pcb->state = RUNNING;
    pcb->priorityLife = pcb->priority;
    pcb->runningCounter++;

    return pcb->stack;
}

void my_nice(uint64_t pid, uint64_t newPrio)
{
    if (newPrio < 0 || newPrio > MAX_PRIORITY)
    {
        printArray("Invalid priority\n");
        return;
    }
    PCB *pcb = PCB_array[pid];
    if (pcb->priority < newPrio)
    {
        increase_priority(pid);
    }
    else if (pcb->priority > newPrio)
    {
        decrease_priority(pid);
    }
    pcb->priority = newPrio;
    pcb->priorityLife = newPrio;
    return;
}

int increase_priority(int pid)
{
    PCB *pcb = PCB_array[pid];
    if (pcb->priority == MAX_PRIORITY)
        return pcb->priority;
    addCircularList(&round_robin, pid);
    pcb->priority++;
    pcb->priorityLife++;
    return pcb->priority;
}

int decrease_priority(int pid)
{
    PCB *pcb = PCB_array[pid];
    if (pcb->priority == 0 || pid == 0 || pid == 1)
        return pcb->priority;
    removeFromCircularList(&round_robin, pid);
    pcb->priority--;
    pcb->priorityLife--;
    return pcb->priority;
}

void tryToExit()
{
    int pid = getCurrentPid();
    PCB *pcb = PCB_array[pid];
    if (pcb->state == FINISHED || pid == 0 || pid == 1)
    {
        printArray("Could not exit process\n");
        return;
    }
    unblockProcess(pcb->ppid);

    killProcess(pid);
}

void my_exit()
{
    tryToExit();
    forceTimerTick();
}

void print_processes()
{
    for (int i = 0; i < processID; i++)
    {
        if (PCB_array[i] != NULL && PCB_array[i]->state != FINISHED)
        {
            printPCB(PCB_array[i]);
        }
    }
}

void loop_print()
{
    while (TRUE)
    {
        printArray("PID: ");
        printDec(current->pid);
        printArray("\n");
        timer_wait(2);
    }
}