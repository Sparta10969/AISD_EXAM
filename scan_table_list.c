#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    char *info;
    Node *next;
} Node;

typedef struct Table {
    Node *head;
} Table;

Node *search(Table *table, int key) {
    if (!table || !table->head) return NULL;

    Node *pointer = table->head;

    for (; pointer; pointer = pointer->next) {
        if (pointer->key == key)
            return pointer;
    }

    return NULL;
}

int insert(Table *table, int key, char *info) {
    if (!table) return -1;

    if (search(table, key)) return 1;

    Node *new_node = calloc(1, sizeof(Node));
    new_node->key = key;
    new_node->info = strdup(info);

    new_node->next = table->head;
    table->head = new_node;

    return 0;
}

int delete(Table *table, int key) {
    if (!table || !table->head) return -1;

    Node *pointer = table->head;
    Node *prev = NULL;
    for (; pointer; pointer = pointer->next) {
        if (pointer->key == key) {
            if (!prev) {
                table->head = pointer->next;
            } else {
                prev->next = pointer->next;
            }
            free(pointer->info);
            free(pointer);
            return 0;
        }
    }

    return 1;
}