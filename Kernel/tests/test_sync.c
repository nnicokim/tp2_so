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
  printArray("LLAMADO A FUNCION DE INCREMENTO\n");
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return 0;

  if ((n = satoi(argv[0])) <= 0)
  {
    my_exit();
    return 0;
  }
  if ((inc = satoi(argv[1])) == 0)
  {
    my_exit();
    return 0;
  }
  if ((use_sem = satoi(argv[2])) < 0)
  {
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
  my_exit();

  return 0;
}

void *my_process_inc_no_sem(int argc, char *argv[])
{
  printArray("LLAMADO A FUNCION DE INCREMENTO\n");
  uint64_t n;
  int8_t inc;

  if (argc != 3)
    return 0;

  if ((n = satoi(argv[0])) <= 0)
  {
    my_exit();
    return 0;
  }
  if ((inc = satoi(argv[1])) == 0)
  {
    my_exit();
    return 0;
  }
  if ((satoi(argv[2])) < 0)
  {
    my_exit();
    return 0;
  }
  uint64_t i;
  for (i = 0; i < n; i++)
  {
    slowInc(&global, inc);
  }

  my_exit();

  return 0;
}

uint64_t test_sync(uint64_t argc, char *argv[])
{

  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 3)
    return 0;

  char *argvDec[] = {argv[0], "-1", argv[1], 0};
  char *argvInc[] = {argv[0], "1", argv[1], 0};

  global = 0;
  uint64_t i;

  if (strcmp(argv[2], "0") == 0)
  {
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      pids[i] = createProcess(namBuf[i], (void *)my_process_inc, 3, argvDec);
      pids[i + TOTAL_PAIR_PROCESSES] = createProcess(namBuf[i], (void *)my_process_inc, 3, argvInc);
    }

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      ksys_waitPid(pids[i]);
      ksys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
    }
  }
  else
  {
    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      pids[i] = createProcess(namBuf[i], (void *)my_process_inc_no_sem, 3, argvDec);
      pids[i + TOTAL_PAIR_PROCESSES] = createProcess(namBuf[i], (void *)my_process_inc_no_sem, 3, argvInc);
    }

    for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
    {
      ksys_waitPid(pids[i]);
      ksys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]);
    }
  }

  printArray("Final value: ");
  printDec(global);
  printArray("\n");
  my_exit();
  return 0;
}