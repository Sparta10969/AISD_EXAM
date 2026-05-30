#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Node {
    int key;
    char *info;
    struct Node *next;
} Node;

typedef struct Table {
    Node **data;
    int size;
} Table;

unsigned long hash(int key) {
    unsigned long hash = 0;
    unsigned char *bytes = (unsigned char *)&key;
    for (int i = 0; i < sizeof(key); i++) {
        hash = 37 * hash + bytes[i];
    }
    return hash;
}

char *search(Table *table, int key) {
    if (!table || !table->data || key < 0 || table->size <= 0) return NULL;

    int index = hash(key) % table->size;
    if (!table->data[index]) return NULL;
    
    for (Node *pointer = table->data[index]; pointer; pointer = pointer->next) {
        if (pointer->key == key) return pointer->info;
    }

    return NULL;
}

int insert(Table *table, int key, char *info) {
    if (!table || !table->data || key < 0) return -1;

    if (search(table, key)) return -1;

    int index = hash(key) % table->size;

    Node *new_node = calloc(1, sizeof(Node));
    if (!new_node) return -1;
    new_node->key = key;
    new_node->info = strdup(info);
    if (!new_node->info) {
        free(new_node);
        return -1;
    }

    new_node->next = table->data[index];
    table->data[index] = new_node;
    
    return 0;
}

int delete(Table *table, int key) {
    if (!table || !table->data || key < 0 || table->size <= 0) return -1;

    if (!search(table, key)) return -1;

    int index = hash(key) % table->size;

    Node *prev = NULL;
    for (Node *pointer = table->data[index]; pointer; pointer = pointer->next) {
        if (pointer->key == key) {
            if (!prev)
                table->data[index] = pointer->next;
            else
                prev->next = pointer->next;
            free(pointer->info);
            free(pointer);
            return 0;
        }
        prev = pointer;
    }

    return 1;
}
