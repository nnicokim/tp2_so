#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define FINISHED 3
#define R_NUM 8

typedef struct PCB
{
    pid_t pid;
    pid_t ppid;
    int state;
    void *stack;
    int priority;

    uint64_t RSP; // puntero al stack donde se pushearon todos los registros/datos
    uint64_t RBP;

    void *baseAddress; // direccion base del proceso (memoria virtual)
    size_t limit;

} PCB;

void initPCB(PCB *pcb, pid_t pid, pid_t ppid, int priority);
void printPCB(PCB *pcb);
int compare_PCB(const PCB *pcb1, const PCB *pcb2);

#endif