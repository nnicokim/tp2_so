#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../../structs/include/pcb.h"
#include "../../structs/include/queue.h"
#include "../../structs/include/circularList.h"

#define PAGE 0x1000
#define MAX_PROCESSES 192 // 64 * 1024

extern Queue PCBqueue;           // Lista de PCBs
extern CircularList round_robin; // Lista de los procesos en round-robin
extern CircularListNode *current;
extern int isSchedulerActive;

void initScheduler();
int blockProcess(int pid);
int unblockProcess(int pid);
int getCurrentPid();
int getCurrentPPid();
CircularListNode *getCurrentProcess();
uint64_t createProcess(char *program, int argc, char **argv);
uint64_t killProcess(int pid);
void *schedule();
void *change_context(int pid);
void my_nice(uint64_t pid, uint64_t newPrio);
int increase_priority(int pid);
int decrease_priority(int pid);
void my_exit();
#endif