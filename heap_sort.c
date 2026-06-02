#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Heap {
    int heap_size;
    int length;
    int *data;
} Heap;

int heap_sort(Heap *heap, int *output) {
    Build_Max_Heap(heap);
    int i = heap->length - 1;

    while (i > 0) {
        int temp = heap->data[i];
        heap->data[i] = heap->data[0];
        heap->data[0] = temp;

        heap->heap_size--;

        Max_Heapify(heap, 0);
        i--;
    }

    if (output)
        memcpy(output, heap->data, heap->length * sizeof(int));
    
    return 0;
}
