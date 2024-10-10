#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../../structs/include/pcb.h"
#include "../../structs/include/stack.h"
#include "../../structs/include/queue.h"

extern Queue PCBqueue;    // Lista de PCBs
extern Queue round_robin; // Lista de los procesos en round-robin

void initScheduler();

#endif