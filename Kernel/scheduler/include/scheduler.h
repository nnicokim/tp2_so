#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../../structs/include/pcb.h"
#include "../../structs/include/stack.h"
#include "../../structs/include/queue.h"
#include "../../structs/include/circularList.h"

extern Queue PCBqueue;           // Lista de PCBs
extern CircularList round_robin; // Lista de los procesos en round-robin

void initScheduler();
int blockProcess(int pid);
int unblockProcess(int pid);
int getCurrentPid();
int getCurrentPPid();
CircularListNode *getCurrentProcess();

#endif