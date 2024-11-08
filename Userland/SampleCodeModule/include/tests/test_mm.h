#ifndef TEST_MM
#define TEST_MM

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <tests/test_util.h>
#include <user_syscalls.h>

void *mymalloc(size_t size);
void myfree(void *ptr);
uint64_t test_mm(uint64_t argc, char *argv[]);

#endif