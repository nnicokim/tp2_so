#include "stack.h"

void initialize(Stack *stack)
{
    stack->top = -1;
}

void push(Stack *stack, int data)
{
    if (isFull(stack))
    {
        fprintf(stderr, "Error: Stack is full.\n");
        exit(1);
    }
    stack->top++;
    stack->data[stack->top] = data;
}

int pop(Stack *stack)
{
    if (isEmpty(stack))
    {
        fprintf(stderr, "Error: Empty stack.\n");
        exit(1);
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
        fprintf(stderr, "Error: Empty stack.\n");
        exit(1);
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
