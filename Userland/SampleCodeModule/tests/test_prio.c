#include "../include/tests/test_prio.h"

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void endless_loop_print(uint64_t wait)
{
    printColor(YELLOW, "endless loop\n");
    while (1)
    {
        bussy_wait(wait);
    }
}

void test_prio()
{
    int fd[] = {0, 1};
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = usys_createProcess("endless_loop_print", endless_loop_print, 0, argv, fd);

    bussy_wait(WAIT);
    printColor(ORANGE, "\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_my_nice(pids[i], prio[i]);

    bussy_wait(WAIT);
    printColor(ORANGE, "\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_blockProcess(pids[i]);

    printColor(ORANGE, "\nCHANGING PRIORITIES...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_my_nice(pids[i], MEDIUM);

    printColor(ORANGE, "\nUNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_unblockProcess(pids[i]);

    bussy_wait(WAIT);
    printColor(ORANGE, "KILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_killProcess(pids[i]);

    putChar('\n');
    printColor(TURQUOISE, "ALL DONE!!!\n");
}