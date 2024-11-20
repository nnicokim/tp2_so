#include "../scheduler/include/scheduler.h"
#include <stdio.h>

void *initStackFrame(void *stackProcess, int argc, char **argv, void *program, uint64_t pid)
{
    StackFrame *newStackFrame = stackProcess;

    newStackFrame->r15 = 0x001;
    newStackFrame->r14 = 0x002;
    newStackFrame->r13 = 0x003;
    newStackFrame->r12 = 0x004;
    newStackFrame->r11 = 0x005;
    newStackFrame->r10 = 0x006;
    newStackFrame->r9 = 0x007;
    newStackFrame->r8 = 0x008;

    newStackFrame->rsi = (uint64_t)argv;
    newStackFrame->rdi = argc;
    newStackFrame->rbp = (uint64_t)stackProcess;
    newStackFrame->rdx = (uint64_t)program;
    newStackFrame->rcx = pid;
    newStackFrame->rbx = 0x014;
    newStackFrame->rax = 0x015;

    newStackFrame->rip = (uint64_t)program;
    newStackFrame->cs = 0x8;
    newStackFrame->rflags = 0x202;
    newStackFrame->rsp = stackProcess;
    newStackFrame->ss = 0x0;

    return newStackFrame->rsp;
}
