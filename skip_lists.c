#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

const size_t MaxLevel = 5;
const double p = 0.5;

typedef struct Item {
    int key;
    int info;
    int level;
    struct Item *forward[MaxLevel];
} Item;

Item EList = {INT_MAX};

typedef struct SkipLists {
    int level;
    Item *header;
} SkipLists;

double random() {
    return ((double)rand() - 1) / RAND_MAX;
}

int generate_random_level() {
    int level = 1;
    while (random() < p && level < MaxLevel)
        level = level + 1;
    return level;
}

Item *search(SkipLists *list, int k) {
    Item *x = list->header;
    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i]->key < k)
            x = x->forward[i];
    }
    x = x->forward[0];
    if (x->key == k) return x;
    return NULL;
}

int insert(SkipLists *list, int k, int info) {
    Item *x = list->header;
    Item *update[list->level];

    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i]->key < k)
            x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    
    if (x->key == k) return 1;

    int level = generate_random_level();
    if (list->level < level) {
        for (int i = list->level; i < level; i++) {
            update[i] = list->header;
        }
        list->level = level;
    }
    
    Item *new_item = calloc(1, sizeof(Item));
    if (!new_item) return -1;
    new_item->key = k;
    new_item->level = level;
    new_item->info = info;

    for (int i = 0; i < level; i++) {
        new_item->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_item;
    }

    return 0;
}

int delete(SkipLists *list, int k) {
    Item *x = list->header;
    Item *update[list->level];

    for (int i = list->level - 1; i >= 0; i--) {
        while (x->forward[i]->key < k)
            x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (x->key != k) return 1;

    for (int i = 0; i < list->level && update[i]->forward[i] == x; i++) {
        update[i]->forward[i] = x->forward[i];
    }

    free(x);

    while (list->level > 1 && list->header->forward[list->level - 1] == &EList)
        list->level--;

    return 0;
}
