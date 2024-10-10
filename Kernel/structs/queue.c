// Cola de procesos (Round-Robin)

#include "./include/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "../memory_manager/include/mm_manager.h"

void initializeQueue(Queue *queue)
{
    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;
}

void addQueue(Queue *queue, PCB *pcb)
{
    QueueNode *newNode = (QueueNode *)mymalloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        return;
    }
    newNode->pcb = pcb;
    newNode->next = NULL;

    if (isEmptyQueue(queue))
    {
        queue->first = newNode;
    }
    else
    {
        queue->last->next = newNode;
    }
    queue->last = newNode;
    queue->size++;
}

PCB *removeQueue(Queue *queue)
{
    if (isEmptyQueue(queue))
    {
        return NULL;
    }

    QueueNode *temp = queue->first;
    PCB *pcb = temp->pcb;

    queue->first = queue->first->next;
    if (queue->first == NULL)
    {
        queue->last = NULL;
    }
    myfree(temp);
    queue->size--;

    return pcb;
}

int isEmptyQueue(Queue *queue)
{
    return queue->first == NULL;
}

PCB *get(Queue *queue, int pid)
{
    QueueNode *current = queue->first;

    while (current != NULL)
    {
        if (current->pcb->pid == pid)
        {
            return current->pcb;
        }
        current = current->next;
    }

    return NULL;
}

void freeQueue(Queue *queue)
{
    while (!isEmptyQueue(queue))
    {
        removeQueue(queue);
    }
}

int queueSize(Queue *queue)
{
    return queue->size;
}