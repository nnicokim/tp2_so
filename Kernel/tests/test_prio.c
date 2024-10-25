#include "./include/test_prio.h"
#include "../include/videoDriver.h"

uint64_t test_prio(uint64_t argc, char *argvAux[])
{
    printArray(" Aunque sea arranca? \n");

    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = createProcess("endless_loop_print", 0, argv);

    bussy_wait(WAIT);
    printArray("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_nice(pids[i], prio[i]);

    bussy_wait(WAIT);
    printArray("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        blockProcess(pids[i]);

    printArray("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_nice(pids[i], MEDIUM);

    printArray("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        unblockProcess(pids[i]);

    bussy_wait(WAIT);
    printArray("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        killProcess(pids[i]);
    rintArray("\nALL DONE!!!\n");
    return 0;
}