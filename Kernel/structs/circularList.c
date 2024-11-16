#include "./include/circularList.h"
#include "../memory_manager/include/mm_manager.h"
#include "../include/videoDriver.h"
#include <time.h>
#include <../memory_manager/include/buddyAllocator.h>

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
    }
    list->size++;

    return;
}

void removeFromCircularList(CircularList *list, int pid)
{
    CircularListNode *current = list->head;

    for (int i = 0; i < list->size; i++)
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

        return;
    }
    printArray("removeFromCircularList: ERROR pid not found\n");
}
