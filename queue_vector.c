#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Queue {
    int *data;
    int head;
    int tail;
    int size;
    int capacity;
} Queue;

int push(Queue *queue, int info) {
    if (!queue || !queue->data) return -1; 
    
    if (queue->size == queue->capacity) return -1;

    queue->data[queue->tail] = info;
    queue->tail = (queue->tail + 1) % queue->capacity; 
    queue->size++;

    return 0;
}

int pop(Queue *queue) {
    if (!queue || !queue->data) return -1;
    if (queue->size == 0) return 1;

    int info = queue->data[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size--;
}
