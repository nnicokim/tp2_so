#include "phylo.h"
#include "./include/interrupts.h"
#include "./include/user_lib.h"
#include <shell.h>
#include <interrupts.h>

static int philosophersCount = 0;
static int state[MAX_PHYL] = {0};
static int phyloPid[MAX_PHYL] = {0};
static char *semNames[MAX_PHYL] = {"s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9"};

int queue[MAX_PHYL];
int queueSize = 0;
int foreground[] = {0, 1};

#define LEFT_PHYLO (i + philosophersCount - 1) % philosophersCount
#define RIGHT_PHYLO (i + 1) % philosophersCount

void think()
{
    bussy_wait(10000000);
}

void eat()
{
    bussy_wait(10000000);
}

void *phylo(int argc, char *argv[])
{

    usys_semOpen("mutex", 1);
    while (1)
    {
        think(); // think
        takeForks(argc);
        eat(); // eat
        putForks(argc);
        printState();
    }

    usys_myExit();
    return NULL;
}

void *phyloProcess(int argc, char *argv[])
{
    char c = 1;
    print("WELCOME TO THE DINING PHILOSOPHERS\n");
    print("press a--> to add a philosophers\n");
    print("press r--> to remove a philosophers\n");
    print("press c--> clear screen\n");
    print("\n------------------------------------\n");
    print("PRESS THE SPACEBAR TO CONTINUE\n");

    while (c != ' ')
    {
        c = getChar();
    }

    usys_clear_screen();
    printColor(GREEN, "philosophers sit to have dinner...\n");

    resetState();
    usys_semOpen("mutex", 1);
    usys_semOpen("countMutex", 1);

    for (int i = 0; i < INIT_PHYL; i++)
    {
        addPhylo();
        queue[i] = -1;
    }

    while (c != 'q')
    {
        c = getChar();
        switch (c)
        {
        case 'a':
            if (addPhylo() == -1)
            {
                printColor(ROJO, "Error adding phylosopher.\n");
            }
            else
            {
                printColor(GREEN, "New phylosopher added.\n");
            }
            break;
        case 'r':
            if (removePhylo() == -1)
            {
                printColor(ROJO, "Error removing philosopher.\n");
            }
            else
            {
                printColor(GREEN, "philosophers removed.\n");
            }
            break;
        case 'c':
            usys_clear_screen();
            print("current state\n");
            break;
        }
    }

    usys_semClose("mutex");
    usys_semClose("countMutex");
    usys_semClose("queueMutex");
    printState();
    endtable();
    usys_myExit();
    return NULL;
}

int addPhylo()
{
    int index = usys_findSem("countMutex");
    usys_semWait(index);
    if (philosophersCount >= MAX_PHYL)
    {
        usys_semPost(index);
        return -1;
    }

    phyloPid[philosophersCount] = usys_createProcess("filosofos", phylo, philosophersCount, NULL, foreground);

    usys_semOpen(semNames[philosophersCount], 1);
    philosophersCount++;
    usys_semPost(index);
    return 1;
}
int removePhylo()
{
    int index = usys_findSem("countMutex");
    usys_semWait(index);
    if (philosophersCount <= MIN_PHYL)
    {
        usys_semPost(index);
        return -1;
    }
    usys_semClose(semNames[philosophersCount - 1]);

    usys_killProcess(phyloPid[philosophersCount - 1]);
    philosophersCount--;
    usys_semPost(index);
    return 1;
}

void endtable()
{
    for (int i = 0; i < philosophersCount; i++)
    {
        usys_killProcess(phyloPid[i]);
    }
}

void printState()
{

    for (int i = 0; i < philosophersCount; i++)
    {
        if (EATING == state[i])
        {
            print("E ");
        }
        else
        {
            print(". ");
        }
    }
    print("\n");
}

void putForks(int i)
{
    int index = usys_findSem("mutex");
    usys_semWait(index);
    state[i] = THINKING;

    int j;
    for (j = 0; j < queueSize; j++)
    {
        int k = queue[j];

        if (state[k] == WAITING && state[(k + philosophersCount - 1) % philosophersCount] != EATING && state[(k + 1) % philosophersCount] != EATING)
        {
            state[k] = EATING;
            int semIndex = usys_findSem(semNames[k]);
            usys_semPost(semIndex);
            int iter;
            for (iter = j; iter < queueSize - 1; iter++)
            {
                queue[iter] = queue[iter + 1];
            }
            queueSize--;
            j--;
        }
    }
    usys_semPost(index);
}

void takeForks(int i)
{
    int index = usys_findSem("mutex");
    usys_semWait(index);
    state[i] = WAITING;

    queue[queueSize++] = i;

    test(i);

    if (state[i] != EATING)
    {
        usys_semPost(index);
        int semIndex = usys_findSem(semNames[i]);
        usys_semWait(semIndex);
    }
    else
    {
        queueSize--;
        usys_semPost(index);
    }
}

void resetState()
{
    for (int i = 0; i < MAX_PHYL; i++)
    {
        state[i] = THINKING;
    }
}

void test(int i)
{
    if (state[i] == WAITING && state[LEFT_PHYLO] != EATING && state[RIGHT_PHYLO] != EATING)
    {
        state[i] = EATING;
        int semIndex = usys_findSem(semNames[i]);
        usys_semPost(semIndex);
    }
}