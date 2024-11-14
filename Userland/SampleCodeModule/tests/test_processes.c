#include "../include/tests/test_processes.h"

enum State
{
    READY,
    RUNNING,
    BLOCKED,
    FINISHED
};

typedef struct P_rq
{
    int32_t pid;
    enum State state;
} p_rq;


void endless_loop()
{
  printColor(WHITE, "Endless loop\n");
  while (1)
    ;
}
int64_t test_processes(uint64_t argc, char *argv[])
{
    printColor(ORANGE, "Testeando procesos...\n");

    int fd[] = {0, 1};
    uint8_t rq;
    uint8_t alive = 0;
    uint8_t action;
    uint64_t max_processes;
    char *argvAux[] = {0};
    int firstime = 1;

    if (argc != 1)
        return -1;

    max_processes = 100;

    p_rq p_rqs[max_processes];

    while (1 && firstime)
    {
        // Create max_processes processes
        for (rq = 0; rq < max_processes; rq++)
        {
            p_rqs[rq].pid = usys_createProcess("endless_lopp", (void *)endless_loop, 0, argvAux, fd);

            if (p_rqs[rq].pid == -1)
            {
                printColor(RED, "test_processes: ERROR creating process \n");
                return -1;
            }
            else
            {
                p_rqs[rq].state = RUNNING;
                alive++;
            }
            printColor(RED, "test_processes: Process created!\n");
        }

        usys_wait(13);

        // Randomly kills, blocks or unblocks processes until every one has been killed
        while (alive > 0)
        {
            if(alive == 1)
            {
                firstime=0;

            }

            for (rq = 0; rq < max_processes; rq++)
            {
                action = GetUniform(100) % 2;

                switch (action)
                {
                case 0:
                    if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED)
                    {
                        if (usys_killProcess(p_rqs[rq].pid) == -1)
                        {
                            printColor(RED, "test_processes: ERROR killing process\n");
                            return -1;
                        }
                        p_rqs[rq].state = FINISHED;
                        
                        alive--;
                    }
                    break;

                case 1:
                    if (p_rqs[rq].state == RUNNING)
                    {
                        if (usys_blockProcess(p_rqs[rq].pid) == -1)
                        {
                            printColor(RED, "test_processes: ERROR blocking process\n");
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
                    if (usys_unblockProcess(p_rqs[rq].pid) == -1)
                    {
                        printColor(RED, "test_processes: ERROR unblocking process\n");
                        return -1;
                    }
                    p_rqs[rq].state = RUNNING;
                }
        }
    }
}