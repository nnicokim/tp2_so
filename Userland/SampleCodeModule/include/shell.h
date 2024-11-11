#ifndef _SHELL_H_
#define _SHELL_H_ 

#include <user_lib.h>
#include <ucolors.h>
#include <eliminator.h>

void init_shell();
void handleCommands(char * str, int * fd);
void handleRegularCommand(char * str, int * fd);

typedef struct Command {
    char* name_id;
    void (*func) (char * param);
    char* desc;
} Command;

#endif