#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Sambo{
    WHITE,
    RED,
    BLUE
}Sambo;

typedef struct Edge{
    int from;
    int to;
    int weight;
}Edge;

typedef struct Vertex{
    int edge_count;
    Edge* edges;
    Sambo color;
    int distance;
    struct Vertex* pred;
    int open;
    int close;
}Vertex;

typedef struct Graph{
    int vertex_count;
    Vertex **vertexes;
}Graph;

typedef struct Queue{
    int capacity;
    int size;
    int head;
    int tail;
    Vertex **data;
}Queue;

#define MAX 100000

Queue* create(int capacity){
    Queue *queue=(Queue*)malloc(sizeof(Queue));
    queue->capacity=capacity;
    queue->size=0;
    queue->data=(Vertex**)malloc(sizeof(Vertex*)*capacity);
    queue->head=queue->tail=0;
    return queue;
}

int pull(Queue *queue, Vertex *val){
    if (queue==NULL) return -1;
    queue->data[queue->tail]=val;
    queue->tail=(queue->tail+1)%queue->capacity;
    queue->size++;
    return 0;
}


int pop(Queue *queue, Vertex **output){
    if (queue==NULL) return -1;
    *output=queue->data[queue->head];
    queue->head=(queue->head+1)%queue->capacity;
    queue->size--;
    return 0;
}

int reset(Graph *graph){
    for (int i=0; i<graph->vertex_count; i++){
        graph->vertexes[i]->color=WHITE;
        graph->vertexes[i]->distance=MAX;
        graph->vertexes[i]->pred=NULL;
        graph->vertexes[i]->open=0;
        graph->vertexes[i]->close=0;
    }
    return 0;
}

int Breadth_First_Search(Graph *graph, Vertex *val){ //поиск в ширину
    reset(graph);
    Queue *queue=create(graph->vertex_count);
    val->distance=0;
    val->color=BLUE;
    pull(queue, val);
    while (queue->size!=0){
        Vertex *v;
        pop(queue, &v);
        for (int i=0; i<v->edge_count; i++){
            int idx=v->edges[i].to;
            Vertex *u=graph->vertexes[idx];
            if (u->color==WHITE){
                u->color=BLUE;
                u->distance=v->distance+1;
                u->pred=v;
                pull(queue, u);
            } 
        }
        v->color=RED;
    }
    free(queue->data);
    free(queue);
    return 0;
}

int visit(Graph *graph, Vertex* u, int* time){
    u->color=BLUE;
    *time= *time+1;
    u->open=*time;
    for (int i=0; i<u->edge_count; i++){
        int ind=u->edges[i].to;
        Vertex *v=graph->vertexes[ind];
        if (v->color==WHITE){
            v->pred=u;
            visit(graph,v,time);
        }
    }
    u->color=RED;
    *time=*time+1;
    u->close=*time;
    return 0;
}


int Depth_First_Search(Graph *graph){
    reset(graph);
    int time=0;
    for (int i=0; i<graph->vertex_count; i++){
        if (graph->vertexes[i]->color==WHITE){
            visit(graph, graph->vertexes[i], &time);
        }
    }
    return 0;
}

typedef struct Stack{
    int capacity; 
    int top;
    Vertex **data;
}Stack;

typedef struct Node{
    Vertex *vertex;
    struct Node* next;
}Node;

typedef struct List{
    struct Node* head;
    int size;
}List;

int create_stack(int capacity){
    Stack* stack=(Stack*)malloc(sizeof(Stack));
    stack->capacity=capacity;
    stack->top=0;
    stack->data=(Vertex**)malloc(sizeof(Vertex*)*capacity);
    return stack;
}

int push(Stack *stack, Vertex* v){
    if (stack==NULL) return -1;  
    stack->data[stack->top]=v;
    stack->top=stack->top+1;
    return -1;
}

int pop_stack(Stack *stack, Vertex **output){
    if (stack==NULL) return -1;
    stack->top=stack->top-1;
    *output=stack->data[stack->top];
    return 0;
}

void free_stack(Stack *stack) {
    if (stack!=NULL) {
        free(stack->data);
        free(stack);
    }
}

int add(List *list, Vertex *v){
    Node* new=(Node*)malloc(sizeof(Node));
    new->vertex=v;
    new->next=list->head;
    list->head=new;
    list->size++;
    return 0;
} 

int topo_visit(Graph *graph, Vertex* u, int* time, Stack *stack){
    u->color=BLUE;
    *time= *time+1;
    u->open=*time;
    for (int i=0; i<u->edge_count; i++){
        int ind=u->edges[i].to;
        Vertex *v=graph->vertexes[ind];
        if (v->color==WHITE){
            v->pred=u;
            visit(graph,v,time);
        }
    }
    u->color=RED;
    *time=*time+1;
    u->close=*time;
    push(stack, u); // add(stack, u); - список связный
    return 0;
}


int topological_sort(Graph *graph){
    reset(graph);
    int time=0;
    Stack *stack=create_stack(graph->vertex_count); //такое же со списком можно сделать
    for (int i=0; i<graph->vertex_count; i++){
        if (graph->vertexes[i]->color==WHITE){
            visit(graph, graph->vertexes[i], &time);
        }
    }
    return 0;
}

int djikstra(Graph *graph, int start){
    reset(graph);
    graph->vertexes[start]->distance=0;
    graph->vertexes[start]->color=BLUE;
    for (int i=0; i<graph->vertex_count; i++){
        int dist=MAX;
        int ind=-1;
        for (int j=0; j<graph->vertex_count; j++){
            if (graph->vertexes[j]->color==BLUE){
                dist=graph->vertexes[j]->distance;
                ind=i;
            }
        }
        if (ind==-1) break;
        Vertex *v=graph->vertexes[ind];
        v->color=RED;
        for (int i=0; i<v->edge_count; i++){
            int weight=v->edges[i].weight;
            int ind=v->edges[i].to;
            Vertex *u=graph->vertexes[ind];
            if (u->color!=RED){
                if (u->distance>v->distance+weight){
                u->distance=v->distance+weight;
                u->pred=v;
                u->color=BLUE;
                }
            }
           
        }
    }
    return 0;
}

int Bellman_Ford(Graph *graph, int start){
    reset(graph);
    graph->vertexes[start]->distance=0;
    graph->vertexes[start]->color=BLUE;
    for (int i=0; i<graph->vertex_count-1; i++){
        for (int j=0; j<graph->vertex_count; j++){
            Vertex* v=graph->vertexes[j];
            for (int k=0; k<v->edge_count; k++){
                int ind=v->edges[k].to;
                int weight=v->edges[k].weight;
                Vertex *u=graph->vertexes[ind];
                if (v->distance+weight<u->distance){
                    u->distance=weight+v->distance;
                    u->pred=v;
                    u->color=BLUE;
                }
            }
        }
    }
    for (int i=0; i<graph->vertex_count; i++){
        Vertex *v=graph->vertexes[i];
        if (v->distance==MAX) continue;
        for (int j=0; j<v->edge_count; j++){
            int ind=v->edges[j].to;
            int weight=v->edges[j].weight;
            Vertex *u=graph->vertexes[ind];
            if (v->distance+weight<u->distance){
                return -1;
            }
        } 
    }
    return 0;
}

int Floyd_Warshall(Graph *graph){

    
}

int CSS(Graph *graph){

}

int Kruskala(Graph *graph){

}

int Prim(Graph *graph){

}

int Ford_Falkerson(Graph *graph){

}