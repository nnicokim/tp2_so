#include "../include/tests/test_prio.h"

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio()
{
    int fd[] = {0, 1};
    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {0};
    uint64_t i;

    printColor(PURPLE, "HOLAAAAA\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        pids[i] = usys_createProcess("endless_loop_print", endless_loop_print, 0, argv, fd);

    bussy_wait(WAIT);
    printColor(ORANGE, "\nCHANGING PRIORITIES...\n");

    printColor(GREEN, "ACA ESTOY\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_my_nice(pids[i], prio[i]);

    printColor(GREEN, "LLEGUE o no?\n");

    bussy_wait(WAIT);
    printColor(ORANGE, "\nKILLING...\n");

    printColor(GREEN, "LLEGUE\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_blockProcess(pids[i]);

    printColor(ORANGE, "\nCHANGING PRIORITIES...\n");

    printColor(TURQUOISE, "Y ACA???\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_my_nice(pids[i], MEDIUM);

    printColor(ORANGE, "\nUNBLOCKING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_unblockProcess(pids[i]);

    bussy_wait(WAIT);
    printColor(ORANGE, "\nKILLING...\n");

    for (i = 0; i < TOTAL_PROCESSES; i++)
        usys_killProcess(pids[i]);

    printColor(TURQUOISE, "ALL DONE!!!\n");
}