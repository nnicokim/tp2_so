#include <stdint.h>
#include <stdio.h>

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#define MAX_NAME 15
#define MAX_SEM 20
#define MAX_SIZE 10

extern int change(int *lock, int value);
void initSems();
int semCreate(char *name, int initValue);
int semOpen(char *name, int initValue);
int semClose(char *name);
int semWait(int semIndex);
int semPost(int semIndex);
char *getSemName(int semIndex);
int findSem(char *name);
void yield();

#endif