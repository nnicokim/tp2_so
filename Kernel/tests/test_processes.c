#include <stdio.h>
#include "include/syscall.h"
#include "include/test_util.h"
#include <videoDriver.h>
#include "../scheduler/include/scheduler.h"
#include <time.h>

typedef struct P_rq
{
    int32_t pid;
    enum State state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[])
{
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {0};

    if (argc != 1)
        return -1;

    if ((max_processes = satoi(argv[0])) <= 0)
        return -1;

    p_rq p_rqs[max_processes];

    while (1)
    {
        clearScreen();

        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++)
        {
            p_rqs[rq].pid = createProcess("endless_loop", 0, argvAux);
            if (p_rqs[rq].pid == -1)
            {
                printArray("test_processes: ERROR creating process: -");
                printDec(p_rqs[rq].pid * -1);
                printArray("\n");
                return -1;
            }
            else
            {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
        }

        timer_wait(2);
        printArray("test_processes: All processes created!!!\n");
        printArray("Ahora: Killing, blocking and unblocking processes\n");

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {

            for (rq = 0; rq < max_processes; rq++)
            {
                action = GetUniform(100) % 2;

                printDec(action);
                printArray(" -> Action.\n");

                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
                    {
                        printArray("Running o Blocked, a matar al proceso: ");
                        printDec(p_rqs[rq].pid);
                        printArray(" !!!\n");
                        // Muere aca la shell (tratando de matar al proceso 13)
                        if (killProcess(p_rqs[rq].pid) == -1)
                        {
                            printArray("test_processes: ERROR killing process with PID: ");
                            printDec(p_rqs[rq].pid);
                            printArray("\n");
                            return -1;
                        }
                        p_rqs[rq].state = FINISHED;
                        alive--;

                        if (alive == 0)
                        {
                            printArray("test_processes: All processes killed!!!\n");
                            timer_wait(2);
                        }
                        if (p_rqs[rq].state == FINISHED)
                        {
                            printArray("Process with PID: ");
                            printDec(p_rqs[rq].pid);
                            printArray(" killed\n");
                        }
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (blockProcess(p_rqs[rq].pid) == -1)
                        {
                            printArray("test_processes: ERROR blocking process with PID: ");
                            printDec(p_rqs[rq].pid);
                            printArray("\n");
                            return -1;
                        }
                        p_rqs[rq].state = BLOCKED;
                    }
                    break;
                }
            }

            // Randomly unblocks processes
            for (rq = 0; rq < max_processes; rq++)
                if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2)
                {
                    if (unblockProcess(p_rqs[rq].pid) == -1)
                    {
                        printArray("test_processes: ERROR unblocking process with PID: ");
                        printDec(p_rqs[rq].pid);
                        printArray("\n");
                        return -1;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
    }
}