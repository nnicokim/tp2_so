#ifndef STACK_H
#define STACK_H

#include <stdint.h>
// #include "./pcb.h"
#include "../memory_manager/include/mm_manager.h"
#include "./circularList.h"

typedef struct StackFrame // El stack crece hacia abajo
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    void *rsp;
    uint64_t ss;
    // El align no hace falta ya que el mm me lo da alineado
} StackFrame;

// typedef struct StackFrame
// {
//     // State of the process
//     uint64_t ss;
//     void *rsp;
//     uint64_t rflags;
//     uint64_t cs;
//     uint64_t rip;

//     uint64_t rax;
//     uint64_t rbx;
//     uint64_t rcx;
//     uint64_t rdx;
//     uint64_t rbp;
//     uint64_t rdi;
//     uint64_t rsi;

//     uint64_t r8;
//     uint64_t r9;
//     uint64_t r10;
//     uint64_t r11;
//     uint64_t r12;
//     uint64_t r13;
//     uint64_t r14;
//     uint64_t r15;
//     // El align no hace falta ya que el mm me lo da alineado
// } StackFrame;

// ChatGPT tira que ese es el tipo de variable de un puntero a un programa que recibe un int y un char **
// void *initStackFrame(void *stackProcess, int argc, char **argv, void (*program)(int, char **), uint64_t pid);
void *initStackFrame(void *stackProcess, int argc, char **argv, void *program, uint64_t pid);

#endif
