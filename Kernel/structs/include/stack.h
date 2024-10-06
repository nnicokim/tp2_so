#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#define MAX 200

typedef struct Stack
{
    int data[MAX];
    int top;
} Stack;

void initialize(Stack *stack);
void push(Stack *stack, int data);
int pop(Stack *stack);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
int peek(Stack *stack);
void clearStack(Stack *stack);
int stackSize(Stack *stack);

#endif
