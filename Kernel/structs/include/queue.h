#ifndef QUEUE_H
#define QUEUE_H

#include "./pcb.h"

typedef struct QueueNode
{
    PCB *pcb;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *first;
    QueueNode *last;
    int size;
} Queue;

void initializeQueue(Queue *queue);
void addQueue(Queue *queue, PCB *pcb);
void removeFromQueue(Queue *queue, int pid);
PCB *deQueue(Queue *queue);
int isEmptyQueue(Queue *queue);
PCB *get(Queue *queue, int pid);
void freeQueue(Queue *queue);
int queueSize(Queue *queue);

#endif