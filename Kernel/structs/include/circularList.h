// Lista circular del Round-robin

#ifndef CIRCULAR_LIST_H
#define CIRCULAR_LIST_H

typedef struct CircularListNode
{
    struct CircularListNode *next;
    struct CircularListNode *prev;
    int pid;
} CircularListNode;

typedef struct CircularList
{
    CircularListNode *head;
    int size;
} CircularList;

extern CircularList round_robin;

// VER SI HAY QUE CREAR UNA STRUCT "ITERATOR" QUE CORRE SOLO COMO SI FUESE EL SCHEDULER !!!!!

void initializeCircularList(CircularList *list);
void addCircularList(CircularList *list, int pid);
void removeFromCircularList(CircularList *list, int pid);

#endif // CIRCULAR_LIST_H
