#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <unistd.h>
#include "./stack.h"

#define KERNEL_PID 0
#define SHELL_PID 1
#define IDLE_PID 2

enum State
{
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
};
// #define READY 0
// #define RUNNING 1
// #define BLOCKED 2
// #define FINISHED 3

typedef struct PCB
{
    unsigned int pid;  // Process ID
    unsigned int ppid; // Parent Process ID
    int state;         // Process state
    void *stack;       // Stack pointer de cada proceso
    int priority;      // Process priority
    unsigned int runningCounter;

    // StackFrame *s_frame; // puntero al stack donde se pushearon todos los registros/datos

    void *baseAddress; // direccion base del proceso (memoria virtual)
    size_t limit;

} PCB;

void initPCB(PCB *pcb, int pid, int ppid, int priority);
PCB *copyPCB(PCB *pcb, PCB *newPCB);
void freePCB(PCB *pcb);
void printPCB(PCB *pcb);
int compare_PCB(const PCB *pcb1, const PCB *pcb2);
void switchToProcess(int processID);

#endif