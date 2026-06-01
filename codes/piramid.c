#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Heap{
    int *data;
    int size;
    int capacity;
}Heap;

Heap* create(int capacity){
    Heap *heap=(Heap*)malloc(sizeof(Heap));
    heap->data=(int*)malloc(sizeof(int)*capacity);
    heap->size=0;
    heap->capacity=capacity;
    return heap;
}

int max_heap(Heap* heap, int i){
    int largest=i;
    int left=2*i+1;
    int right=2*i+2;
    if (left<heap->size && heap->data[left]>heap->data[largest]) largest=left;
    if (right<heap->size && heap->data[right]>heap->data[largest]) largest=right;
    if (largest!=i){
        int time=heap->data[i];
        heap->data[i]=heap->data[largest];
        heap->data[largest]=time;
        max_heap(heap, largest);
    }
    return 0;
}

void build_max_heap(Heap* heap){
    for (int i=heap->size/2-1; i>=0; i--){
        max_heap(heap, i);
    }
}

int max_up(Heap* heap, int cur){
    while (cur>0){
        int par=(cur-1)/2; //i
        if (heap->data[cur]>heap->data[par]){
            int time=heap->data[cur];
            heap->data[cur]=heap->data[par];
            heap->data[par]=time;
            cur=par;
        }else{
            break;
        }
    }
    return 0;
}

int heap_sort(Heap *heap){
    build_smax_heap(heap);
    int i=heap->size;
    while (i>0){
        int time=heap->data[0];
        heap->data[0]=heap->data[i];
        heap->data[i]=time;
        heap->size--;
        max_heapify(heap,0);
    }
    return 0;
}