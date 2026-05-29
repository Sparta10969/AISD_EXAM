#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    char *info;
} Node;

typedef struct Table {
    Node *data;
    int capacity;
    int size;
} Table;

int search(Table *table, int key) {
    if (!table || !table->data) return -1;
    if (table->size == 0) return -1;

    int end = table->size - 1;
    int start = 0;
    int i = 0;

    while(start <= end) {
        i = (start + end) / 2;
        if (table->data[i].key == key)
            return i;
        else if (table->data[i].key < key) {
            start = i + 1;
        } else {
            end = i - 1;
        }
    }

    return -1;
}

int insert(Table *table, int key, char *info) {
    if (!table || !table->data) return -1;

    if (search(table, key) >= 0) return 1;
    if (table->size == table->capacity) return 2;

    Node new_node;
    new_node.key = key;
    new_node.info = strdup(info);

    int i = table->size - 1;
    while (i >= 0 && table->data[i].key > key) {
        table->data[i + 1] = table->data[i];
        i--;
    }

    table->data[i + 1] = new_node;
    table->size++;

    return 0;
}

int delete(Table *table, int key) {
    if (!table || !table->data) return -1;
    if (table->size == 0) return -1;

    int index = search(table, key);
    if (index < 0) return 1;

    free(table->data[index].info);

    for (int i = index; i < table->size - 1; i++) {
        table->data[i] = table->data[i + 1];
    }

    table->size--;

    return 0;
}
