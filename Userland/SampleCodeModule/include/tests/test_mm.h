#ifndef TEST_MM_H
#define TEST_MM_H

#include <stdio.h>
#include <stdlib.h>
#include "./syscall.h"
#include "./test_util.h"

#define MAX_BLOCKS 128

uint64_t test_mm(uint64_t argc, char *argv[]);

#endif