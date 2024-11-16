#include <stdint.h>
#include "../interrupts.h"

int64_t my_getpid();
int64_t my_create_process(char *name, uint64_t argc, char *argv[]);
int64_t not_my_nice(uint64_t pid, uint64_t newPrio);
int64_t my_kill(uint64_t pid);
int64_t my_block(uint64_t pid);
int64_t my_unblock(uint64_t pid);
int semCreate(char *name, int initValue);
int semOpen(char *name, int initValue);
int semClose(char *name);
int semWait(int semIndex);
int semPost(int semIndex);
void yield();
