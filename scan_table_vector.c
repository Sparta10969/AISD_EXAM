#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    if (table->size == 0 || !table->data || table->capacity == 0)
        return -1;

    for (int i = 0; i < table->size; i++) {
        if (table->data[i].key == key)
            return i;
    }

    return -1;
}

int insert(Table *table, int key, char *info) {
    if (!table || !table->data) return -1;

    if (search(table, key) >= 0)
        return 1;

    if (table->size == table->capacity)
        return 2;

    Node new_item;
    new_item.key = key;
    new_item.info = strdup(info);

    table->data[table->size] = new_item;
    table->size += 1;

    return 0;
}

int delete(Table *table, int key) {
    if (!table || !table->data) return -1;

    if (table->size == 0)
        return 1;

    int index = search(table, key);
    if (index < 0)
        return 2;

    free(table->data[index].info);
    table->data[index].key = table->data[table->size - 1].key;
    table->data[index].info = table->data[table->size - 1].info;
    table->size -= 1;
    
    return 0;
}