#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;

typedef struct Queue{
    struct Node* head;
    struct Node* tail;
}Queue;

int push(Queue *q, int v){
    if (q==NULL) return 1;
    Node *node=(Node*)malloc(sizeof(Node));
    node->data=v;
    node->next=NULL;
    if (q->tail==NULL){
        q->tail=node;
        q->head=node;
    }else{
        q->tail=node;
        q->tail->next=node;
    }
    return 0;
}

int pull(Queue* q,int* out){
    if (q==NULL || q->head==NULL || q->tail==NULL) return 1;
    Node *time=q->head;
    *out=time->data;
    q->head=time->next;
    free(time);
    if (q->head==NULL) q->tail==NULL;
    return 0;
}

Queue* create(void){
    Queue *q=(Queue*)malloc(sizeof(Queue));
    q->head=q->tail=NULL;
    return q;
}

void kill(Queue *q){
    while (q->head!=NULL){
        Node *time=q->head->next;
        free(q->head);
        q->head=time;
    }
    free(q);
}