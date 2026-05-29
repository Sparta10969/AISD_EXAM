#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    int info;
    struct Node *next;
} Node;

typedef struct Stack {
    Node *top;
} Stack;

int push(Stack *stack, int info) {
    if (!stack) return -1;

    Node *new_node = calloc(1, sizeof(Node));
    if (!new_node) return -1;
    new_node->info = info;

    new_node->next = stack->top;
    stack->top = new_node;

    return 0;
}

int pop(Stack *stack) {
    if (!stack || !stack->top) return NULL;

    Node *x = stack->top;
    stack->top = x->next; 

    int info = x->info;
    free(x);

    return info;
}

