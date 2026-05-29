#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
    int info;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *tail;
} Queue;

int push(Queue *queue, int info) {
    if (!queue) return -1;

    Node *new_node = calloc(1, sizeof(Node));
    if (!new_node) return -1;
    new_node->info = info;

    if (!queue->tail) {
        queue->tail = new_node;
        new_node->next = new_node;
    } else {
        new_node->next = queue->tail->next;
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    return 0;
}

int pop(Queue *queue) {
    if (!queue || !queue->tail) return -1;

    Node *x = queue->tail->next;

    if (x == queue->tail) {
        queue->tail = NULL;
    } else {
        queue->tail->next = x->next;
    }

    int info = x->info;
    free(x);

    return info;
}
