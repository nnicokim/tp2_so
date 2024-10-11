#ifndef STACK_H
#define STACK_H

#define MAX 192 // 64 * 3

typedef struct Stack
{
    int data[MAX];
    int top;
} Stack;

void initStack(Stack *stack);
void push(Stack *stack, int data);
int pop(Stack *stack);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
int peek(Stack *stack);
void clearStack(Stack *stack);
int stackSize(Stack *stack);

#endif
