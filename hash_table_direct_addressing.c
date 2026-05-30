#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    int key;
    char *info;
} Node;

typedef struct Table {
    Node **data;
    int size;
    int capacity;
} Table;

char *search(Table *table, int key) {
    if (!table || !table->data || table->size <= 0 || key >= table->capacity) return NULL;

    Node *node = table->data[key];

    if (node) return node->info;
    return NULL;
}

int insert(Table *table, int key, char *info) {
    if (!table || key < 0 || key >= table->capacity) return 1;

    if (search(table, key)) return 2;

    Node *new_item = calloc(1, sizeof(Node));
    if (!new_item) return -1;
    new_item->key = key;
    new_item->info = strdup(info);
    if (!new_item->info) {
        free(new_item);
        return -1;
    }

    table->data[key] = new_item;
    table->size++;

    return 0;
}

int delete(Table *table, int key) {
    if (!table || key < 0 || key >= table->capacity || table->size <= 0) return -1;

    if (!search(table, key)) return 1;

    free(table->data[key]->info);
    free(table->data[key]);

    table->data[key] = NULL;
    table->size--;

    return 0;
}
