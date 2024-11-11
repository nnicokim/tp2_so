#include <stdint.h>
#include "../include/tests/test_util.h"
#include "../include/string.h"
#include "../ipc/include/semaphore.h"
#include <videoDriver.h>
#include "../include/tests/syscall.h"
#include "../scheduler/include/scheduler.h"
#include "../include/tests/test_sync.h"

extern void forceTimerTick();

uint64_t ksys_waitPid(int pid);

#define SEM_ID "sem"
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory
char namBuf[][3] = {"R1", "R2", "R3", "R4"};

void *slowInc(int64_t *p, int64_t inc)
{
  uint64_t aux = *p;
  printArray("| ");
  printDec(aux);
  printArray("+ ( ");
  printDec(inc);
  printArray(" ) = ");
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
  printDec(*p);
  printArray("|\n");
  return 0;
}

void *my_process_inc(int argc, char *argv[])
{
  printArray("ENTRO A: my_process_inc\n");
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
  {
    printArray("Sale por argc != 3\n");
    my_exit();
    return 0;
  }

  if ((n = satoi(argv[0])) <= 0)
  {
    printArray("Sale por n = satoi(argv[0])) <= 0 n\n");
    my_exit();
    return 0;
  }
  if ((inc = satoi(argv[1])) == 0)
  {
    printArray("Sale por inc = satoi(argv[1])) == 0\n");
    my_exit();
    return 0;
  }
  if ((use_sem = satoi(argv[2])) < 0)
  {
    printArray("Sale por use_sem = satoi(argv[2])) < 0\n");
    my_exit();
    return 0;
  }
  if (use_sem)
    if (semOpen(SEM_ID, 1) == -1)
    {
      printArray("test_sync: ERROR opening semaphore\n");
      my_exit();
      return 0;
    }

  printArray("use_sem: ");
  printDec(use_sem);
  printArray("\n");
  printArray("Valor de n:");
  printDec(n);
  printArray("\n");

  uint64_t i;
  for (i = 0; i < n; i++)
  {
    if (use_sem)
      semWait(findSem(SEM_ID));
    slowInc(&global, inc);
    if (use_sem)
      semPost(findSem(SEM_ID));
  }

  if (use_sem)
    semClose(SEM_ID);

  printArray("SALIENDO de my_process_inc...\n");
  my_exit();

  return 0;
}

void *my_process_inc_no_sem(int argc, char *argv[])
{
  printArray("ENTRO A: my_process_inc_no_sem\n");
  uint64_t n;
  int8_t inc;

  if (argc != 3)
  {
    printArray("Sale por argc != 3\n");
    my_exit();
    return 0;
  }

  if ((n = satoi(argv[0])) <= 0)
  {
    printArray("Sale por n = satoi(argv[0])) <= 0\n");
    my_exit();
    return 0;
  }
  if ((inc = satoi(argv[1])) == 0)
  {
    printArray("Sale por inc = satoi(argv[1])) == 0\n");
    my_exit();
    return 0;
  }
  if ((satoi(argv[2])) < 0)
  {
    printArray("Sale por satoi(argv[2])) < 0\n");
    my_exit();
    return 0;
  }
  uint64_t i;
  for (i = 0; i < n; i++)
  {
    slowInc(&global, inc);
  }

  printArray("SALIENDO de my_process_inc_no_sem...\n");
  my_exit();

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[])
{
  int fd[]={0,1};
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3)
  {
    my_exit();
    return 0;
  }

  char *argvDec[] = {argv[0], "-1", argv[1], 0};
  char *argvInc[] = {argv[0], "1", argv[1], 0};

  global = 0;
  uint64_t i;

  if (strcmp(argv[2], "0") == 0)
  {
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      pids[i] = createProcess(namBuf[i], (void *)my_process_inc, 3, argvDec, fd);
      pids[i + TOTAL_PAIR_PROCESSES] = createProcess(namBuf[i], (void *)my_process_inc, 3, argvInc, fd);
    }

    printArray("Waiting for processes to finish del (strcmp(argv[2], 0) == 0)...\n");

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      if (ksys_waitPid(pids[i]) == -1)
        printArray("ERROR WAITING FOR PROCESS\n");

      if (ksys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]) == -1)
        printArray("ERROR WAITING FOR PROCESS\n");
    }

    printArray("Termino de esperar a los procesos...\n");
  }
  else
  {
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      pids[i] = createProcess(namBuf[i], (void *)my_process_inc_no_sem, 3, argvDec, fd);
      pids[i + TOTAL_PAIR_PROCESSES] = createProcess(namBuf[i], (void *)my_process_inc_no_sem, 3, argvInc, fd);
    }

    printArray("Waiting for processes to finish (en el ELSE)...\n");

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      if (ksys_waitPid(pids[i]) == -1)
        printArray("ERROR WAITING FOR PROCESS\n");

      if (ksys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]) == -1)
        printArray("ERROR WAITING FOR PROCESS\n");
    }

    printArray("Termino de esperar a los procesos...\n");
  }

  printArray("Final value: ");
  printDec(global);
  printArray("\n");

  printArray("SALIENDO del test_sync...\n");
  my_exit();
  return 0;
}