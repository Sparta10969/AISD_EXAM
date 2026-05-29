#include <stdio.h>
#include <stdlib.h>

typedef struct Stack{
    int top;
    int* data;
    int size;
}Stack;

void create(int size){
    Stack *s=(Stack*)malloc(sizeof(int)*size);
    s->size=size;
    s->data=malloc(sizeof(int));
    s->top=0;
    return 0;
}
int push(Stack *s, int v){
    if (s==NULL || s->size<=0) return 1;
    if (s->top>=s->size-1) return 1;
    s->data[s->top]=v;
    s->top++;
    return 0;
}

int pull(Stack *s, int *out){
    if (s->top<=0 || s==NULL) return 1;
    s->top--;
    *out=s->data[s->top];
    return 0;
}

void kill(Stack *s){
    if (s==NULL) return;
    free(s->data);
    free(s);
    return 0;
}