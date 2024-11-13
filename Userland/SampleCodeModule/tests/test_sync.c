// #include <stdint.h>
// #include "../include/tests/test_util.h"
// #include "../include/tests/test_sync.h"

// // extern void forceTimerTick();

// #define SEM_ID "sem"
// #define TOTAL_PAIR_PROCESSES 2

// int64_t global; // shared memory
// char namBuf[][3] = {"R1", "R2", "R3", "R4"};
// char num[3] = "0";

// void *slowInc(int64_t *p, int64_t inc)
// {
//   uint64_t aux = *p;
//   print("| ");
//   intToStr(aux, num);
//   print(aux);
//   print("+ ( ");
//   intToStr(inc, num);
//   print(inc);
//   print(" ) = ");
//   yield(); // This makes the race condition highly probable
//   aux += inc;
//   *p = aux;
//   intToStr(*p, num);
//   print(*p);
//   print("|\n");
//   return 0;
// }

// void *my_process_inc(int argc, char *argv[])
// {

//   printColor(TURQUOISE, "ENTRO A: my_process_inc\n");
//   uint64_t n;
//   int8_t inc;
//   int8_t use_sem;

//   if (argc != 3)
//   {
//     usys_myExit();
//     return 0;
//   }

//   if ((n = satoi(argv[0])) <= 0)
//   {
//     usys_myExit();
//     return 0;
//   }
//   if ((inc = satoi(argv[1])) == 0)
//   {
//     usys_myExit();
//     return 0;
//   }
//   if ((use_sem = satoi(argv[2])) < 0)
//   {
//     usys_myExit();
//     return 0;
//   }
//   if (use_sem)
//     if (semOpen(SEM_ID, 1) == -1)
//     {
//       printColor(RED, "test_sync: ERROR opening semaphore\n");
//       usys_myExit();
//       return 0;
//     }

//   print("use_sem: ");
//   intToStr(use_sem, num);
//   print(use_sem);
//   print("\n");
//   print("Valor de n:");
//   intToStr(n, num);
//   print(n);
//   print("\n");

//   uint64_t i;
//   for (i = 0; i < n; i++)
//   {
//     if (use_sem)
//       semWait(findSem(SEM_ID));
//     slowInc(&global, inc);
//     if (use_sem)
//       semPost(findSem(SEM_ID));
//   }

//   if (use_sem)
//     semClose(SEM_ID);

//   print("SALIENDO de my_process_inc...\n");
//   usys_myExit();

//   return 0;
// }

// void *my_process_inc_no_sem(int argc, char *argv[])
// {
//   print("ENTRO A: my_process_inc_no_sem\n");
//   uint64_t n;
//   int8_t inc;

//   if (argc != 3)
//   {
//     print("Sale por argc != 3\n");
//     usys_myExit();
//     return 0;
//   }

//   if ((n = satoi(argv[0])) <= 0)
//   {
//     print("Sale por n = satoi(argv[0])) <= 0\n");
//     usys_myExit();
//     return 0;
//   }
//   if ((inc = satoi(argv[1])) == 0)
//   {
//     print("Sale por inc = satoi(argv[1])) == 0\n");
//     usys_myExit();
//     return 0;
//   }
//   if ((satoi(argv[2])) < 0)
//   {
//     print("Sale por satoi(argv[2])) < 0\n");
//     usys_myExit();
//     return 0;
//   }
//   uint64_t i;
//   for (i = 0; i < n; i++)
//   {
//     slowInc(&global, inc);
//   }

//   print("SALIENDO de my_process_inc_no_sem...\n");
//   usys_myExit();

//   return 0;
// }

// uint64_t test_sync(uint64_t argc, char *argv[])
// {
//   int fd[] = {0, 1};
//   uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

//   if (argc != 3)
//   {
//     usys_myExit();
//     return 0;
//   }

//   char *argvDec[] = {argv[0], "-1", argv[1], 0};
//   char *argvInc[] = {argv[0], "1", argv[1], 0};

//   global = 0;
//   uint64_t i;

//   if (strcmp(argv[2], "0") == 0)
//   {
//     for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
//     {
//       pids[i] = usys_createProcess(namBuf[i], (void *)my_process_inc, 3, argvDec, fd);
//       pids[i + TOTAL_PAIR_PROCESSES] = usys_createProcess(namBuf[i], (void *)my_process_inc, 3, argvInc, fd);
//     }

//     print("Waiting for processes to finish del (strcmp(argv[2], 0) == 0)...\n");

//     for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
//     {
//       if (usys_waitPid(pids[i]) == -1)
//         printColor(RED, "ERROR WAITING FOR PROCESS\n");

//       if (usys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]) == -1)
//         printColor(RED, "ERROR WAITING FOR PROCESS\n");
//     }

//     print("Termino de esperar a los procesos...\n");
//   }
//   else
//   {
//     for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
//     {
//       pids[i] = usys_createProcess(namBuf[i], (void *)my_process_inc_no_sem, 3, argvDec, fd);
//       pids[i + TOTAL_PAIR_PROCESSES] = usys_createProcess(namBuf[i], (void *)my_process_inc_no_sem, 3, argvInc, fd);
//     }

//     print("Waiting for processes to finish (en el ELSE)...\n");

//     for (i = 0; i < TOTAL_PAIR_PROCESSES; i++)
//     {
//       if (usys_waitPid(pids[i]) == -1)
//         printColor(RED, "ERROR WAITING FOR PROCESS\n");

//       if (usys_waitPid(pids[i + TOTAL_PAIR_PROCESSES]) == -1)
//         printColor(RED, "ERROR WAITING FOR PROCESS\n");
//     }

//     print("Termino de esperar a los procesos...\n");
//   }

//   print("SALIENDO del test_sync...\n");
//   usys_myExit();
//   return 0;
// }