// Cola de procesos (Round-Robin)

#include <queue.h>

void initializeQueue(Queue *queue)
{
    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;
}

void addQueue(Queue *queue, PCB *pcb)
{
    // implementar algun malloc nuestro si NO se puede usar el de la libreria

    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL)
    {
        fprintf(stderr, "Error: Cannot add to Queue.\n");
        exit(1);
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
        fprintf(stderr, "Error: Empty Queue.\n");
        exit(1);
    }

    QueueNode *temp = queue->first;
    PCB *pcb = temp->pcb;

    queue->first = queue->first->next;
    if (queue->first == NULL)
    {
        queue->last = NULL;
    }
    free(temp);
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