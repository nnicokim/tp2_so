#include "./include/stack.h"
#include <stdio.h>

void initializeStack(Stack *stack)
{
    stack->top = -1;
}

void push(Stack *stack, int data)
{
    if (isFull(stack))
    {
        return;
    }
    stack->top++;
    stack->data[stack->top] = data;
}

int pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        return -1;
    }
    int toReturn = stack->data[stack->top];
    stack->top--;
    return toReturn;
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

int isFull(Stack *stack)
{
    return stack->top == MAX - 1;
}

int peek(Stack *stack)
{
    if (isEmpty(stack))
    {
        return -1;
    }
    return stack->data[stack->top];
}

void clearStack(Stack *stack)
{
    stack->top = -1;
}

int stackSize(Stack *stack)
{
    return stack->top + 1;
}
