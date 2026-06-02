#include <stdio.h>
#include <stdlib.h>

typedef struct Queue{
    int size;
    int capacity;
    int head;
    int* data;
    int tail;
}Queue;

Queue* create(int capacity){
    Queue *q=(Queue*)malloc(sizeof(Queue));
    q->data=malloc(sizeof(int));
    q->size=0;
    q->capacity=capacity;
    q->tail=q->head=0;
    return q;
}

int push(Queue* q, int v){
    if (q==NULL || q->capacity<q->size || q->capacity<=0) return 1;
    q->data[q->tail]=v;
    q->tail=(q->tail+1)%q->capacity;
    q->size++;
    return 0;
}

int pull(Queue *q, int* out){
    if (q==NULL || q->size>=q->capacity || q->capacity<=0) return 1;
    *out=q->data[q->head];
    q->head=(q->head+1)%q->capacity;
    q->size--;
    return 0;
}

void kill(Queue *q){
    free(q->data);
    free(q);
}