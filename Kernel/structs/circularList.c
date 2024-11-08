#include "./include/circularList.h"
#include "../memory_manager/include/mm_manager.h"
#include "../include/videoDriver.h"
#include <time.h>

void initializeCircularList(CircularList *list)
{
    list->head = NULL;
    list->size = 0;
}

void addCircularList(CircularList *list, int pid)
{
    CircularListNode *newNode = (CircularListNode *)mymalloc(sizeof(CircularListNode));
    if (newNode == NULL)
    {
        printArray("addCircularList: ERROR creating CircularListNode\n");
        return;
    }
    newNode->pid = pid;
    if (list->head == NULL)
    {
        list->head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        CircularListNode *prev_head = list->head;
        newNode->prev = prev_head->prev;
        prev_head->prev->next = newNode;

        newNode->next = prev_head;
        prev_head->prev = newNode;

        list->head = newNode;

        // CircularListNode *last = list->head;
        // list->head = newNode;
        // last->next = newNode;
        // newNode->next = last->prev;
        // last->prev->next = newNode;

        // newNode->prev = last;
        // last->next = newNode;

        // // newNode->prev = last;
        // // newNode->next = list->head;
        // // list->head->prev = newNode;
    }
    list->size++;

    return;
}

void removeFromCircularList(CircularList *list, int pid)
{
    CircularListNode *current = list->head;
    if (list->size == 0 || list->head == NULL)
    {
        printArray("removeFromCircularList: ERROR: CircularList is empty\n");
        return;
    }

    for (int i = 0; i <= list->size; i++)
    {
        if (current->pid != pid)
        {
            current = current->next;
            continue;
        };

        CircularListNode *prev = current->prev;
        CircularListNode *next = current->next;
        prev->next = next;
        next->prev = prev;
        if (current == list->head)
        {
            list->head = next;
        }
        myfree(current);
        list->size--;
        if (list->size == 0)
        {
            list->head = NULL;
        }
        return;
    }
    printArray("El proceso con pid: ");
    printDec(pid);
    printArray(" -> No se encontro del circularList.\n");
    return;
}
