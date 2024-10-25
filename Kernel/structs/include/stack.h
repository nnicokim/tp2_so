#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include "./pcb.h"
#include "../memory_manager/include/mm_manager.h"
#include "../scheduler/include/scheduler.h"
#include "./circularList.h"

#define MAX 192 // 64 * 3. Si se pasa, deberia tirar algun error al usuario.

typedef struct StackFrame
{
    // State of the process
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi; // argv
    uint64_t rdi; // argc
    uint64_t rbp;
    uint64_t rdx; // program
    uint64_t rcx; // pid
    uint64_t rbx;
    uint64_t rax;
    uint64_t rip;    // loader
    uint64_t cs;     // 0x8
    uint64_t rflags; // 0x202
    uint64_t rsp;    // currentProcess->RSP
    uint64_t ss;     // 0
    // El align no hace falta ya que el mm me lo da alineado
} StackFrame;
typedef struct Stack
{
    StackFrame stackFrames[MAX];
    int top;
} Stack;

void initStack(Stack *stack);
void push(Stack *stack, StackFrame data);
StackFrame pop(Stack *stack);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
int peek(Stack *stack);
void clearStack(Stack *stack);
int stackSize(Stack *stack);
extern void save_context(StackFrame *frame);
extern void load_context(StackFrame *frame);

// ChatGPT tira que ese es el tipo de variable de un puntero a un programa que recibe un int y un char **
void initStackFrame(int argc, char **argv, void (*program)(int, char **), uint64_t pid);

#endif
