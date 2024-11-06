#include "../scheduler/include/scheduler.h"
#include <stdio.h>

void *initStackFrame(void *stackProcess, int argc, char **argv, void (*program)(int, char **), uint64_t pid)
{
    StackFrame *newStackFrame = stackProcess;
    // Escribe desde la direccion de memoria de stackProcess.
    // Una vez que termine de asignar el stackFrame, el RSP va a ser la direccion contigua a este stackFrame.
    newStackFrame->r15 = 0x001;
    newStackFrame->r14 = 0x002;
    newStackFrame->r13 = 0x003;
    newStackFrame->r12 = 0x004;
    newStackFrame->r11 = 0x005;
    newStackFrame->r10 = 0x006;
    newStackFrame->r9 = 0x007;
    newStackFrame->r8 = 0x008;
    newStackFrame->rsi = 0x009; // se repite
    newStackFrame->rdi = 0x010; // se repite
    newStackFrame->rbp = 0x011; // se repite
    newStackFrame->rdx = 0x012; // se repite
    newStackFrame->rcx = 0x013; // se repite
    newStackFrame->rbx = 0x014;
    newStackFrame->rax = 0x015; // Push/Pop state

    // Registros necesarios para la interrupcion
    newStackFrame->rsi = (uint64_t)argv;
    newStackFrame->rdi = argc;
    newStackFrame->rbp = 0;
    newStackFrame->rdx = (uint64_t)program;
    newStackFrame->rcx = pid;
    newStackFrame->rip = (uint64_t)program;
    newStackFrame->cs = 0x8;
    newStackFrame->rflags = 0x202;
    newStackFrame->rsp = stackProcess + sizeof(StackFrame); // Lo puse igual que el RSP del PCB
    newStackFrame->ss = 0x0;                                // Iretq

    return stackProcess + sizeof(StackFrame);
}
