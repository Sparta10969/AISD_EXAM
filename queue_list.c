#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
    int info;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *head;
    Node *tail;
} Queue;

int push(Queue *queue, int info) {
    if (!queue) return -1;

    Node *new_node = calloc(1, sizeof(Node));
    if (!new_node) return -1;
    new_node->info = info;

    if (queue->head == NULL && queue->tail == NULL) {
        queue->head = new_node;
        queue->tail = new_node;
        return 0;
    }

    queue->tail->next = new_node;
    queue->tail = new_node;

    return 0;
}

int pop(Queue *queue) {
    if (!queue || !queue->head) return -1;

    Node *x = queue->head;
    queue->head = x->next;

    int info = x->info;
    free(x);

    if (queue->head == NULL)
        queue->tail = NULL;

    return info;
}
