#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    int key;
    char *info;
    int busy; // 0 - свободна, 1 - занята, 2 - удалена
} Node;

typedef struct Table {
    Node **data;
    int size;
    int capacity;
} Table;

unsigned long hash1(int key) {
    unsigned long hash = 0;
    unsigned char *bytes = (unsigned char *)&key;
    for (int i = 0; i < sizeof(key); i++) {
        hash = 37 * hash + bytes[i];
    }
    return hash;
}

unsigned long hash2(int key) {
    unsigned long hash = 0;
    unsigned char *bytes = (unsigned char *)&key;
    for (int i = 0; i < sizeof(key); i++) {
        hash = 131 * hash + bytes[i];
    }
    return hash;
}

char *search(Table *table, int key) {
    if (!table || !table->data || key < 0 || table->size <= 0) return NULL;

    int hashed1 = hash1(key) % table->capacity;
    int hashed2 = hash2(key) % table->capacity;
    int index = hashed1;
    int n = 0;
    while (table->data[index] && table->data[index]->busy != 0 && n < table->capacity) {
        if (table->data[index]->busy == 1 && table->data[index]->key == key)
            return table->data[index]->info;
        n++;
        index = (hashed1 + n * hashed2) % table->capacity;
    }
    
    return NULL;
}

int insert(Table *table, int key, char *info) {
    if (!table || !table->data || key < 0) return -1;

    if (search(table, key)) return -1;

    int hashed1 = hash1(key) % table->capacity;
    int hashed2 = hash2(key) % table->capacity;
    int index = hashed1;
    int n = 0;
    while (table->data[index] && table->data[index]->busy == 1 && n < table->capacity) {
        n++;
        index = (hashed1 + n * hashed2) % table->capacity;
    }

    if (n < table->capacity) {
        Node *new_item = calloc(1, sizeof(Node));
        if (!new_item) return -1;
        new_item->key = key;
        new_item->busy = 1;
        new_item->info = strdup(info);
        if (!new_item->info) {
            free(new_item);
            return -1;
        }

        if (table->data[index]) {
            free(table->data[index]->info);
            free(table->data[index]);
        }

        table->data[index] = new_item;
        table->size++;

        return 0;
    }
    return 1;
}

int delete(Table *table, int key) {
    if (!table || !table->data || key < 0 || table->size <= 0)  return -1;

    if (!search(table, key)) return -1;

    int hashed1 = hash1(key) % table->capacity;
    int hashed2 = hash2(key) % table->capacity;
    int index = hashed1;
    int n = 0;
    while (table->data[index] && table->data[index]->busy != 0 && n < table->capacity) {
        if (table->data[index]->key == key) {
            table->data[index]->busy = 2;
        }
        n++;
        index = (hashed1 + n * hashed2) % table->capacity;
    }

    return 0;
}
