#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    int key;
    int info;
    int busy; // 0 - свободна, 1 - занята
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
        if (table->data[i].busy == 1 && table->data[i].key == key)
            return i;
    }

    return -1;
}

int insert(Table *table, int key, char *info) {
    if (!table || !table->data) return -1;

    if (search(table, key) >= 0)
        return 1;

    if (table->size == table->capacity) {
        collect(table);

        if (table->size == table->capacity)
            return 2;
    }

    Node new_item;
    new_item.key = key;
    new_item.info = strdup(info);

    table->data[table->size] = new_item;
    table->size++;

    return 0;
}

int collect(Table *table) {
    int i = 0, j = 0;

    for (int i = 0; i < table->size; i++) {
        if (table->data[i].busy == 1) {
            table->data[j] = table->data[i];
            table->data[i].busy = 0;
            j++;
        } else {
            if (table->data[i].info) free(table->data[i].info);
        }
    }

    table->size = j;
    return j;
}

int delete(Table *table, int key) {
    if (!table || !table->data) return -1;

    if (table->size == 0)
        return 1;

    int index = search(table, key);
    if (index < 0)
        return 2;

    free(table->data[index].info);
    table->data[index].busy = 0;
    
    return 1;
}