#include <pcb.h>

void initPCB(PCB *pcb, pid_t pid, pid_t ppid, int priority)
{
    pcb->pid = pid;
    pcb->ppid = ppid;
    pcb->state = READY;
    pcb->priority = priority;
    pcb->RSP = 0;
    pcb->RBP = 0;

    for (int i = 0; i < R_NUM; i++)
    {
        pcb->registers[i] = 0;
    }

    pcb->baseAddress = NULL;
    pcb->limit = 0;
}

void printPCB(PCB *pcb)
{
    printf("PID: %d\n", pcb->pid);
    printf("PPID: %d\n", pcb->ppid);
    printf("State: %d\n", pcb->state);
    printf("Priority: %d\n", pcb->priority);
    printf("RSP: %d\n", pcb->RSP);
    printf("RBP: %d\n", pcb->RBP);

    printf("Registers:\n");
    for (int i = 0; i < R_NUM; i++)
    {
        printf("R%d: %d\n", i, pcb->registers[i]);
    }

    printf("Base Address: %p\n", pcb->baseAddress);
    printf("Limit: %d\n", pcb->limit);
}

int compare_PCB(const PCB *pcb1, const PCB *pcb2)
{
    return pcb1->pid - pcb2->pid;
}