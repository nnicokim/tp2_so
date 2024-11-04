#include "../scheduler/include/scheduler.h"
#include <stdio.h>

void *initStackFrame(void *stackProcess, int argc, char **argv, void (*program)(int, char **), uint64_t pid)
{
    PCB *currentProcess = &PCB_array[pid];
    StackFrame *stack = (StackFrame *)(currentProcess->s_frame);
    stack->r15 = 0x001;
    stack->r14 = 0x002;
    stack->r13 = 0x003;
    stack->r12 = 0x004;
    stack->r11 = 0x005;
    stack->r10 = 0x006;
    stack->r9 = 0x007;
    stack->r8 = 0x008;
    // Registros necesarios para la interrupcion
    stack->rsi = (uint64_t)argv;
    stack->rdi = argc;
    stack->rbp = 0;
    stack->rdx = (uint64_t)program;
    stack->rcx = pid;
    stack->rip = (uint64_t)program;
    stack->cs = 0x8;
    stack->rflags = 0x202;
    stack->rsp = (uint64_t)(stack->rsp);
    stack->ss = 0x0;

    return stackProcess + sizeof(StackFrame);
}
