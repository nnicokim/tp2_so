#ifndef TEST_PROCESSES_H
#define TEST_PROCESSES_H

#include <stdint.h>
#include "./test_util.h"
#include "../interrupts.h"
#include "../user_lib.h"

int64_t test_processes(uint64_t argc, char *argv[]);

#endif