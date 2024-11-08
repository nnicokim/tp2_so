#include <tests/test_prio.h>
#include "../include/tests/test_prio.h"

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio()
{
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = usys_createProcess("endless_loop_print", 0, argv);

    bussy_wait(WAIT);
    printColor(LIGHT_BLUE, "\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_my_nice(pids[i], prio[i]);

    bussy_wait(WAIT);
    printColor(LIGHT_BLUE, "\nBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_blockProcess(pids[i]);

    printColor(LIGHT_BLUE, "CHANGING PRIORITIES WHILE BLOCKED...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        _my_nice(pids[i], MEDIUM);

    printColor(LIGHT_BLUE, "UNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_unblockProcess(pids[i]);

    bussy_wait(WAIT);
    printColor(RED, "\nKILLING.....\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_killProcess(pids[i]);
    printColor(GREEN, "ALL DONE!!!\n");
}