#ifndef TEST_PRIO_H
#define TEST_PRIO_H

#include <stdint.h>
#include <stdio.h>
#include "./syscall.h"
#include "./test_util.h"
// #include "../scheduler/include/scheduler.h"
#include <user_syscalls.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0  // TODO: Change as required
#define MEDIUM 1  // TODO: Change as required
#define HIGHEST 2 // TODO: Change as required

// uint64_t test_prio(uint64_t argc, char *argvAux[]); // originalmente era de tipo void y sim params
void test_prio();
#endif