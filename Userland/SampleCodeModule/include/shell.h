#ifndef _SHELL_H_
#define _SHELL_H_

#include <user_lib.h>
#include <ucolors.h>
#include <eliminator.h>
#include <user_syscalls.h>
#include "./tests/test_processes.h"
#include "./tests/test_prio.h"
#include "./tests/test_mm.h"

void init_shell();

typedef struct Command
{
    char *name_id;
    void (*func)(char *param);
    char *desc;
} Command;

#endif