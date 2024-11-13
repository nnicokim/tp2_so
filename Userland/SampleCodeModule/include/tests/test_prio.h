#ifndef TEST_PRIO_H
#define TEST_PRIO_H

#include <stdint.h>
#include <stdio.h>
#include "./syscall.h"
#include "./test_util.h"
#include "../interrupts.h"

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 10000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 0
#define MEDIUM 1
#define HIGHEST 2

void test_prio();
#endif