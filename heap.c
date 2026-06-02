#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Heap {
    int heap_size;
    int length;
    int *data;
} Heap;

int Max_Heapify(Heap *heap, int index) {
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

int Build_Max_Heap(Heap *heap) {
    int i = heap->heap_size / 2 - 1;
    while (i >= 0) {
        Max_Heapify(heap, i);
        i--;
    }
    return 0;
}
