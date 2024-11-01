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
        CircularListNode *last = list->head->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = list->head;
        list->head->prev = newNode;
    }
    list->size++;
    printArray("Size de la lista luego del ADD: ");
    printDec(list->size);
    printArray("\n");
    timer_wait_ms(50);
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
        if (current->pid != pid) // Se muere aca despues de la 1ra comparacion
        {
            current = current->next;
            if (pid == 13)
            {
                printArray("Sigo buscando al PROCESO 13...\n");
            }
            continue;
        };

        if (pid == 13)
        {
            printArray("Encontre EL PROCESO 13 de la CIRCULAR LIST.\n");
        }
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
