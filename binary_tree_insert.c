#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
    int key;
    Node *left;
    Node *right;
    Node *parent;
} Node;

typedef struct Tree {
    Node *root;
} Tree;

int insert(Tree *tree, int key) {
    if (!tree) return -1;

    Node *new_node = calloc(1, sizeof(Node));
    new_node->key = key;

    if (!tree->root) {
        tree->root = new_node;
    }

    Node *pointer = tree->root;
    Node *parent = NULL;
    while (pointer) {
        parent = pointer;
        if (key == pointer->key) {
            free(new_node);
            return -1;
        } else if (key < pointer->key) {
            pointer = pointer->left;
        } else {
            pointer = pointer->right;
        }
    }

    if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    new_node->parent = parent;

    return 0;
}
