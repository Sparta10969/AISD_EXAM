#include <stdio.h>
#include <stdlib.h>

typedef struct Data{
    int data;
    struct Data* next;
}Data;

typedef struct Stack{
    struct Data* top;
}Stack;

int push(Stack *s, int v){
    if (s==NULL) return 1;
    Data* node=(Data*)malloc(sizeof(Data));
    node->data=v;
    node->next=s->top;
    s->top=node;
    return 0;
}

int pull(Stack *s, int *out){
    if (s==NULL || s->top==NULL) return 1;
    Data *time=s->top;
    *out=time->data;
    s->top=time->next;
    free(time);
    return 0;
}

Stack* create(void){
    Stack* s=(Stack*)malloc(sizeof(Stack));
    s->top=NULL;
    return s;
}

int kill(Stack *s){
    Data *cur=s->top;
    while (cur){
        Data* next=cur->next;
        free(cur);
        cur=next;
    }
    free(s);
}