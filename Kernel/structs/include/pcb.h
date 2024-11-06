#ifndef PCB_H
#define PCB_H

#include <stdint.h>
#include <unistd.h>
#include "./stack.h"
#include "../../scheduler/include/scheduler.h"

#define SHELL_PID 0
#define IDLE_PID 1

#define MAX_PROCESSES 128 // 64 * 1024

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

    void *baseAddress; // direccion base del proceso (memoria virtual)
    size_t limit;

} PCB;

extern PCB *PCB_array[MAX_PROCESSES]; // Array de PCBs

void initPCB(PCB *pcb, int pid, int ppid, int priority);
PCB *copyPCB(PCB *pcb, PCB *newPCB);
void freePCB(PCB *pcb);
int compare_PCB(const PCB *pcb1, const PCB *pcb2);
void printPCB(PCB *pcb);

#endif