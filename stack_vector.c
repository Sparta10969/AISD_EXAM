#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Stack {
    int *data;
    int top;
    int size;
} Stack;

int push(Stack *stack, int info) {
    if (!stack || !stack->data) return -1;
    if (stack->size == stack->top) return 1;

    stack->data[stack->top] = info;
    stack->top += 1;

    return 0;
}

int pop(Stack *stack) {
    if (!stack || !stack->data || stack->top <= 0) return -1;

    stack->top--;
    return stack->data[stack->top];
}
