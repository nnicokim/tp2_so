#include "./include/circularList.h"
#include "../memory_manager/include/mm_manager.h"

void initializeCircularList(CircularList *list)
{
    list->head = NULL;
    list->size = 0;
}

void addCircularList(CircularList *list, int pid)
{
    CircularListNode *newNode = (CircularListNode *)mymalloc(sizeof(CircularListNode));
    newNode->pid = pid;
    if (list->head == NULL)
    {
        list->head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    }
    else
    {
        CircularListNode *last = list->head->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = list->head;
        list->head->prev = newNode;
    }
    list->size++;
}

void removeCircularList(CircularList *list, int pid)
{
    CircularListNode *current = list->head;
    if (list->size == 1)
    {
        myfree(current);
        list->head = NULL;
        list->size = 0;
        return;
    }
    for (int i = 0; i < list->size; i++)
    {
        if (current->pid == pid)
        {
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
        current = current->next;
    }
}
