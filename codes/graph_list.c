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

typedef struct Elem{
    Node *vertex;
    int priority;
}Elem;

typedef struct Priority_Queue{
    int capacity;
    int size;
    struct Elem *data;
}Priority_Queue;

Priority_Queue* create_pq(int capacity){
    Priority_Queue* queue=(Priority_Queue*)malloc(sizeof(Priority_Queue));
    if (queue==NULL) return NULL;
    queue->capacity=capacity;
    queue->size=0;
    queue->data=(Elem*)malloc(sizeof(Elem));
    if (queue->data==NULL){
        free(queue);
        return NULL;
    }
    queue->data->vertex=NULL;
    queue->data->priority=0;
    return queue;
}

int min_down(Priority_Queue *queue){
    int cur=queue->size-1;
    while(cur>0){
        int par=(cur-1)/2;
        if (queue->data[cur].priority<queue->data[par].priority){
            Elem time=queue->data[cur];
            queue->data[cur]=queue->data[par];
            queue->data[par]=time;
            cur=par;
        }else{
            break;
        }
    }
    return 0;
}

int max_up(Priority_Queue *queue){
    int cur=0;
    while(1){
        int par=cur;
        int left=2*cur+1;
        int right=2*cur+2;
        if (left<queue->size && queue->data[left].priority<queue->data[par].priority) par=left;
        if (right<queue->size && queue->data[right].priority<queue->data[par].priority) par=right;
        if (cur==par) break;
        Elem time=queue->data[cur];
        queue->data[cur]=queue->data[par];
        queue->data[par]=time;
        cur=par;
    }
    return 0;
}

int pull(Priority_Queue *queue, int priority, Node *vertex){
    if (queue==NULL || vertex==NULL) return -1;
    if (queue->size>=queue->capacity){
        int new_capacity=queue->capacity*2;
        Elem *time=(realloc(queue->data, sizeof(Elem)*new_capacity));
        if (time==NULL) return -1;
        queue->data=time;
        queue->capacity=new_capacity;
    }
    queue->data[queue->size].vertex=vertex;
    queue->data[queue->size].priority=priority;
    queue->size++;
    min_down(queue);
    return 0;
}

int get(Priority_Queue *queue, Elem *node){
    if (queue==NULL) return -1;
    if (queue->size==0) return -1;
    node->vertex=queue->data[0].vertex;
    node->priority=queue->data[0].priority;
    return 0;
}

int extract_min(Priority_Queue *queue, Elem *node){
    if (queue==NULL) return -1;
    if (queue->size==0) return -1;
    node->vertex=queue->data[0].vertex;
    node->priority=queue->data[0].priority;
    queue->size--;
    queue->data[0]=queue->data[queue->size];
    max_up(queue);
    return 0;
}

int decrease_key(Priority_Queue *queue, Elem *node){
    if (queue==NULL || node==NULL) return -1;
    for (int i=0; i<queue->size; i++){
        if (queue->data[i].vertex==node->vertex){
            queue->data[i].priority=node->priority;
            int cur=i;
            while(cur>0){
                int par=(cur-1)/2;
                if (queue->data[cur].priority<queue->data[par].priority){
                    Elem time=queue->data[cur];
                    queue->data[cur]=queue->data[par];
                    queue->data[par]=time;
                    cur=par;
                }else{
                    break;
                }
            }
            return 0;
        }
    }
    return -1;
}

void free_pq(Priority_Queue* pq) {
    if (pq!=NULL) {
        free(pq->data);
        free(pq);
    }
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
                if (graph->vertexes[j]->distance<dist){ 
                    dist=graph->vertexes[j]->distance;
                    ind=i;
                }
            }
        }
        if (ind==-1) break;
        Vertex *v=graph->vertexes[ind];
        v->color=RED;
        for (int k=0; k<v->edge_count; k++){
            int weight=v->edges[k].weight;
            int ind=v->edges[k].to;
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

int djikstra_2(Graph *graph, int start){
    reset(graph);
    graph->vertexes[start]->distance=0;
    graph->vertexes[start]->color=BLUE;
    Priority_Queue* queue=create_pq(graph->vertex_count);
    for (int i=0; i<graph->vertex_count; i++){
        pull(queue, graph->vertexes[i]);
    }
    while (queue!=NULL){
        Vertex *v;
        if (extract_min(queue,&v)!=0) break;
        v->color=RED;
        for (int k=0; k<v->edge_count; k++){
            int weight=v->edges[k].weight;
            int ind=v->edges[k].to;
            Vertex *u=graph->vertexes[ind];
            if (u->color!=RED){
                if (u->distance>v->distance+weight){
                u->distance=v->distance+weight;
                u->pred=v;
                u->color=BLUE;
                decrease_key(queue,u);
                }
            }
        } 
    }  
    free_pq(queue);
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
    int **matr=(int**)malloc(sizeof(int*)*graph->vertex_count);
    for (int i=0; i<graph->vertex_count; i++){
        matr[i]=(int*)malloc(sizeof(int)*graph->vertex_count);
    }

    for (int i=0; i<graph->vertex_count; i++){
        for (int j=0; j<graph->vertex_count; j++){
            if (i==j){
                matr[i][j]=0;
            }else{
                matr[i][j]=MAX;
            }
        }
    }
    for (int i=0; i<graph->vertex_count; i++){
        for (int k=0; k<graph->vertexes[i]->edge_count; k++){
            int j=graph->vertexes[i]->edges[k].to;
            int weight=graph->vertexes[i]->edges[k].weight;
            if (weight<matr[i][j]){
                matr[i][j]=weight;
            }
        }
    }

    for (int k=0; k<graph->vertex_count; k++){
        for (int i=0; i<graph->vertex_count; i++){
            for (int j=0; j<graph->vertex_count; j++){
                if (matr[i][k]<MAX && matr[k][j]<MAX){
                    if (matr[i][k]+matr[k][j]<matr[i][j]){
                        matr[i][j]=matr[i][k]+matr[k][j];
                    }
                }
            }
        }
    }

    for (int i=0; i<graph->vertex_count; i++){
        free(matr[i]);
    }
    free(matr);
    return 0;
}

int transpone(Graph *graph){
    if (graph==NULL) return -1;
    int V=graph->vertex_count;
    Graph *gt=(Graph*)malloc(sizeof(Graph));
    gt->vertex_count=V;
    gt->vertexes=(Vertex**)malloc(sizeof(Vertex*) * V);
    int *deg=(int*)calloc(V,sizeof(int)); //степени вершин
    for (int i=0; i<V; i++){
        for (int j=0; j<graph->vertexes[i]->edge_count; j++){
            int to=graph->vertexes[i]->edges[j].to;
            deg[to]++;
        }
    }
    for (int i=0; i<V; i++){ //память под всю эту эпопею
        gt->vertexes[i]=(Vertex*)malloc(sizeof(Vertex));
        gt->vertexes[i]->edge_count=deg[i];
        if (deg[i]>0){
            gt->vertexes[i]->edges=(Edge*)malloc(sizeof(Edge)*deg[i]);
        } else {
            gt->vertexes[i]->edges=NULL;
        }
        reset(gt);
        deg[i]=0;
    }
    // заполняем ребра в обратном направлении
    for (int i=0; i<V; i++){
        for (int j=0; j<graph->vertexes[i]->edge_count; j++){
            int to=graph->vertexes[i]->edges[j].to;
            int weight=graph->vertexes[i]->edges[j].weight;
            int idx=deg[to];
            gt->vertexes[to]->edges[idx].from=to;
            gt->vertexes[to]->edges[idx].to=i;
            gt->vertexes[to]->edges[idx].weight=weight;
            deg[to]++;
        }
    }
    free(deg);
    return gt;
}

void visit_scc(Graph *tran_graph, int idx, int id){
    Vertex *u=tran_graph->vertexes[idx];
    u->color=BLUE;
    u->distance=id; 
    for (int i=0; i<u->edge_count; i++){
        int j=u->edges[i].to;
        if (tran_graph->vertexes[j]->color==WHITE) {
            visit_scc(tran_graph,j,id);
        }
    }
    u->color=RED;
}

typedef struct {
    int index;
    int close_time;
} VertexOrder;

int compare_close_time(const void *a, const void *b) {
    VertexOrder *va=(VertexOrder *)a;
    VertexOrder *vb=(VertexOrder *)b;
    return vb->close_time-va->close_time;
}

int CSS(Graph *graph){
    Depth_First_Search(graph);
    Graph* tran_graph=tanspone(graph);
    VertexOrder *order=(VertexOrder*)malloc(sizeof(VertexOrder)*graph->vertex_count);
    for (int i=0; i<graph->vertex_count; i++){
        order[i].index=i;
        order[i].close_time=graph->vertexes[i]->close;
    }
    qsort(order, graph->vertex_count, sizeof(int), compare_close_time);
    int count=0;
    for (int i=0; i<graph->vertex_count; i++){
        int idx=order[i].index;
        if (tran_graph->vertexes[idx]->color==WHITE) {
            visit_scc(tran_graph,tran_graph->vertexes[idx],count);
            count++;
        }
    }
    for (int i=0; i<graph->vertex_count; i++) {
        if (tran_graph->vertexes[i]->edges!=NULL){
            free(tran_graph->vertexes[i]->edges);
        }
        free(tran_graph->vertexes[i]);
    }
    free(tran_graph->vertexes);
    free(tran_graph);
    return 0;
}

int cmp_edges(const void *a, const void *b) {
    Edge *e1=(Edge *)a;
    Edge *e2=(Edge *)b;
    return e1->weight-e2->weight;
}

int Kruskala(Graph *graph){
    int V=graph->vertex_count;
    int A=0;
    for (int i=0; i<V; i++){
        A+=graph->vertexes[i]->edge_count;
    }
    Edge* all=(Edge*)malloc(sizeof(Edge*)*A); //все рёбра
    int ind=0;
    for (int i=0; i<V; i++){
        for (int j=0; j<graph->vertexes[i]->edge_count; j++){
            all[ind]=graph->vertexes[i]->edges[j];
            ind++;
        }
    }
    qsort(all, A, sizeof(Edge), cmp_edges);
    int *par=(int*)malloc(sizeof(int)*V);
    for (int i=0; i<V; i++) par[i]=i;
    int used=0;
    for (int i=0; i<A && used<V-1; i++){
        int u=all[i].from;
        int v=all[i].to;
        int root=u;
         while (par[root]!=root) root=par[root];
        int root_v=v;
        while (par[root_v]!=root_v) root_v=par[root_v];
        
        if (root!=root_v) {
            par[root]=root_v;
            used++;
        }
    }
    
    free(all);
    free(par);
    return 0;
}

int Prim(Graph *graph){
    reset(graph);
    graph->vertexes[0]->distance=0;
    for (int i=0; i<graph->vertex_count; i++){
        int dist=MAX;
        int ind=-1;
        for (int j=0; j<graph->vertex_count; j++){
            if (graph->vertexes[j]->color!=RED){
                if (graph->vertexes[j]->distance<dist){ 
                    dist=graph->vertexes[j]->distance;
                    ind=i;
                }
            }
        }
        if (ind==-1) break;
        graph->vertexes[ind]->color = RED;
        for (int k=0; k<graph->vertexes[ind]->edge_count; k++) {
            int v=graph->vertexes[ind]->edges[k].to;
            int weight=graph->vertexes[ind]->edges[k].weight;
            if (graph->vertexes[v]->color!=RED && weight<graph->vertexes[v]->distance) {
                graph->vertexes[v]->distance=weight;
                graph->vertexes[v]->pred=graph->vertexes[ind];
            }
        }
    }
    return 0;
}

int get_vertex_index(Graph *graph, Vertex *v){
    for (int i=0; i<graph->vertex_count; i++){
        if (graph->vertexes[i]==v) return i;
    }
    return -1;
}

int Ford_Falkerson(Graph *graph){
    int V=graph->vertex_count;
    if (V<2) return 0;
    int s=0; // исток 
    int t=V-1; // сток

    // матрицы пропускных способностей и потоков
    int **cap=(int**)malloc(sizeof(int*)*V);
    int **flow=(int**)malloc(sizeof(int*)*V);
    for (int i=0; i<V; i++){
        cap[i]=(int*)calloc(V, sizeof(int));
        flow[i]=(int*)calloc(V, sizeof(int));
    }
    for (int i=0; i<V; i++){
        for (int j=0; j<graph->vertexes[i]->edge_count; j++){
            int u=graph->vertexes[i]->edges[j].from;
            int v=graph->vertexes[i]->edges[j].to;
            int w=graph->vertexes[i]->edges[j].weight;
            cap[u][v]+=w;
        }
    }
    int max_flow=0;
    int *parent=(int*)malloc(sizeof(int)*V);
    // пока существует увеличивающий путь в остаточной сети
    while (1){
        for (int i=0; i<V; i++) parent[i]=-1;
        parent[s]=s; 
        
        // пихаем в очередь Queue
        Queue *queue=create(V);
        pull(queue, graph->vertexes[s]);
        // поиск в ширину (BFS) для нахождения кратчайшего увеличивающего пути
        while (queue->size!=0){
            Vertex *u_ptr;
            pop(queue, &u_ptr);
            int u=get_vertex_index(graph, u_ptr);
            if (u==t) break; // нашли сток
            for (int v=0; v<V; v++){
                // остаточная пропускная способность cf(u, v) = cap[u][v] - flow[u][v]
                if (parent[v]==-1 && (cap[u][v]-flow[u][v])>0){
                    parent[v]=u;
                    pull(queue, graph->vertexes[v]);
                }
            }
        }
        
        // освобождаем очередь
        free(queue->data);
        free(queue);
        if (parent[t]==-1){
            break;
        }
        // находим остаточную пропускную способность пути cf(p)
        int path_flow=MAX;
        int v=t;
        while (v!=s){
            int u=parent[v];
            int residual=cap[u][v]-flow[u][v];
            if (residual<path_flow){
                path_flow=residual;
            }
            v=u;
        }
        // увеличиваем поток вдоль найденного пути
        v=t;
        while (v!=s){
            int u=parent[v];
            flow[u][v]+=path_flow;
            flow[v][u]-=path_flow; // антисимметричность потока
            v=u;
        } 
        max_flow+=path_flow;
    }
    free(parent);
    for (int i=0; i<V; i++){
        free(cap[i]);
        free(flow[i]);
    }
    free(cap);
    free(flow);
    return max_flow;
}