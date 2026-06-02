#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct Node {
    int key;
} Node;

typedef struct PriorityQueue {
    int heap_size;
    int length;
    int *data;
} PriorityQueue;

int Max_Heapify(PriorityQueue *heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    int largest = index;

    if (left < heap->heap_size && heap->data[left] > heap->data[largest]) largest = left;
    if (right < heap->heap_size && heap->data[right] > heap->data[largest]) largest = right;
    
    if (largest != index) {
        int temp = heap->data[index];
        heap->data[index] = heap->data[largest];
        heap->data[largest] = temp;

        Max_Heapify(heap, largest);
    }

    return 0;
}

int Extract_Maximum(PriorityQueue *queue) {
    if (queue->heap_size == 0) return 1;

    int res = queue->data[0];
    queue->data[0] = queue->data[queue->heap_size - 1];
    queue->heap_size--;

    Max_Heapify(queue, 0);

    return res;
}

int Increase_Key(PriorityQueue *queue, int index, int key) {
    if (index >= queue->heap_size || key < queue->data[index])
        return 1;

    queue->data[index] = key;

    while (index > 0 && queue->data[(index - 1) / 2] < queue->data[index]) {
        int parent = (index - 1) / 2;
        int temp = queue->data[index];
        queue->data[index] = queue->data[parent];
        queue->data[parent] = temp;

        index = parent;
    }

    return 0;
}

int Insert(PriorityQueue *queue, int key) {
    if (queue->heap_size + 1 > queue->length) return 1;

    queue->heap_size++;
    queue->data[queue->heap_size - 1] = INT_MIN;

    return Increase_Key(queue, queue->heap_size - 1, key);
}

int Maximum(PriorityQueue *queue, int *res) {
    if (queue->heap_size < 1) return 1;
    *res = queue->data[0];
    return 0;
}
