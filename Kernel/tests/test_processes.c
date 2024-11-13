#include <stdio.h>
#include <tests/test_util.h>
#include <tests/syscall.h>
#include <videoDriver.h>
#include "../scheduler/include/scheduler.h"
#include "../include/time.h"

typedef struct P_rq
{
    int32_t pid;
    enum State state;
} p_rq;

int64_t test_processes(uint64_t argc, char *argv[])
{
    int fd[] = {0, 1};
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {0};

    int counter = 0;

    if (argc != 1)
        return -1;

    if ((max_processes = satoi(argv[0])) <= 0)
        return -1;

    p_rq p_rqs[max_processes];

    while (1)
    {
        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++)
        {
            p_rqs[rq].pid = createProcess("endless_lopp", (void *)endless_loop, 0, argvAux, fd);

            if (p_rqs[rq].pid == -1)
            {
                printArray("test_processes: ERROR creating process: -");
                return -1;
            }
            else
            {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
            printArray("test_processes: Process created with PID: ");
            printDec(p_rqs[rq].pid);
            printArray("\n");
        }

        timer_wait_ms(10);

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {

            for (rq = 0; rq < max_processes; rq++)
            {
                action = GetUniform(100) % 2;

                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
                    {
                        if (killProcess(p_rqs[rq].pid) == -1)
                        {
                            printArray("test_processes: ERROR killing process with PID: ");
                            printDec(p_rqs[rq].pid);
                            printArray("\n");
                            return -1;
                        }
                        p_rqs[rq].state = FINISHED;
                        alive--;
                        counter++;
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (blockProcess(p_rqs[rq].pid) == -1)
                        {
                            printArray("test_processes: ERROR blocking process with PID: ");

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

                        return -1;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }

        // printDec(counter);
        // printArray("\n");
        // return 0;
    }
}