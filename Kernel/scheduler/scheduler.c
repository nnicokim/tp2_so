// Algoritmo: Round-Robin con prioridades
#include <videoDriver.h>
#include "./include/scheduler.h"

extern void forceTimerTick();

int isSchedulerActive = 0;
Stack *stack;

CircularListNode *current = NULL;
static int processID = 3;

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
    initPCB(&newPCB, processID, getCurrentPid(), 1); // ver que prioridad se le pasa
    addQueue(&PCBqueue, &newPCB);

    // Stack nuevo -> RSP = RBP
    newPCB.stack = initStackFrame(newStack, argc, argv, (void *)program, processID);
    processID++;
    newPCB.baseAddress = newStack;
    newPCB.limit = PAGE;

    newPCB.s_frame = newStackFrame;

    // Añado al scheduler
    addCircularList(&round_robin, newPCB.pid);
    printArray("Se creo el proceso con PID: ");
    printDec(newPCB.pid);
    printArray(" :) \n");

    return newPCB.pid;
}

void idleProcess()
{
    while (TRUE)
    {
        _hlt();
    }
}

void createIdleProcess(void (*f)())
{
    StackFrame *newStackFrame = mymalloc(sizeof(StackFrame));
    void *newStack = mymalloc(PAGE);
    // StackFrame
    if (newStackFrame == NULL || newStack == NULL)
    {
        printArray("createProcess: ERROR creating process. Could not allocate Stack for process.");
        printDec(processID);
        printArray("\n");
        return;
    }

    PCB PCBidle;
    initPCB(&PCBidle, IDLE_PID, KERNEL_PID, 0);
    addQueue(&PCBqueue, &PCBidle);

    // Stack nuevo -> RSP = RBP
    PCBidle.stack = initStackFrame(newStack, 0, NULL, f, IDLE_PID);
    PCBidle.baseAddress = newStack;
    PCBidle.limit = PAGE;

    PCBidle.s_frame = newStackFrame;

    // Añado al scheduler
    addCircularList(&round_robin, IDLE_PID);
    printArray("Se creo el proceso IDLE :) \n");
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

    // agrego al Round-robin asi vuelve a correr este proceso (con prioridades)
    for (int i = 0; i <= pcb->priority; i++)
        addCircularList(&round_robin, pcb->pid);
    printArray("Se agrego al Round-robin en ChangeContext.\n");

    // Siguiente proceso a correr
    current = current->next != NULL ? current->next : round_robin.head;
}

// Debemos devolver el RSP del proceso que comienza a correr
void *schedule() // void *
{
    sleepCurrent();
    return change_context(current->pid);
}

void *change_context(int pid)
{
    PCB *pcb = get(&PCBqueue, pid);
    printArray("Cambiando contexto del proceso con PID: ");
    printDec(current->pid);
    printArray("\n");
    while (pcb->state == BLOCKED)
    { // Me salteo todos los procesos bloqueados hasta llegar al proximo proceso READY
        current = current->next != NULL ? current->next : round_robin.head;
        pcb = get(&PCBqueue, current->pid);
    }
    pcb->state = RUNNING;
    pcb->runningCounter++;

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
    if (pcb->priority == MAX_PRIORITY)
        return pcb->priority;
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