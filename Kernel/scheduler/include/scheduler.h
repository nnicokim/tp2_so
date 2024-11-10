#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../../structs/include/pcb.h"
#include "../../structs/include/circularList.h"
#include "../structs/include/stack.h"
#include "../memory_manager/include/mm_manager.h"
#include <interrupts.h>

#define PAGE 0x1000
#define TRUE 1
#define MAX_PRIORITY 5
#define MIN_PRIORITY 0
#define DEFAULT_PRIORITY 1

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
void idleProcess();
uint64_t createOneProcess();
void randomFunction();
uint64_t killProcess(int pid);
void *schedule(void *rsp);
void *change_context(int pid);
void my_nice(uint64_t pid, uint64_t newPrio);
int increase_priority(int pid);
int decrease_priority(int pid);
void my_exit();
void print_processes();
void loop_print();

#endif