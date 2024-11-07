/* sampleCodeModule.c */

#include <user_syscalls.h>
#include <user_lib.h>
#include <shell.h>
#include <stdio.h>

int main()
{
	// print("hola\n");
	// usys_createProcess(init_shell, 0, NULL);
	init_shell();
	return 0;
}