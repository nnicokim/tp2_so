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

int64_t test_processes(uint64_t argc, char *argv[])
{
    printColor(ORANGE, "Testeando procesos...\n");

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
                        counter++;
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
        char buff[10];
        intToStr(counter, buff);
        print("Counter: ");
        print(buff);
        print("\n");
    }
}