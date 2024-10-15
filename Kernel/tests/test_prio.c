#include "./include/test_prio.h"
#include "../include/videoDriver.h"

void test_prio()
{
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = my_create_process("endless_loop_print", 0, argv);

    bussy_wait(WAIT);
    printArray("\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_nice(pids[i], prio[i]);

    bussy_wait(WAIT);
    printArray("\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_block(pids[i]);

    printArray("CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_nice(pids[i], MEDIUM);

    printArray("UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_unblock(pids[i]);

    bussy_wait(WAIT);
    printArray("\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        my_kill(pids[i]);
}