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

Node *search_recursive(Node *node, int key) {
    if (!node) return NULL;

    if (node->key == key)
        return node;
    if (key < node->key)
        return search_recursive(node->left, key);
    else
        return search_recursive(node->right, key);
}

Node *search_iterative(Node *node, int key) {
    Node *pointer = node;
    while (pointer) {
        if (key == pointer->key)
            return pointer;
        if (key < pointer->key)
            pointer = pointer->left;
        else
            pointer = pointer->right;
    }

    return NULL;
}

Node *min(Node *node) {
    Node *pointer = node;
    while (pointer->left) pointer = pointer->left;
    return pointer;
}

Node *max(Node *node) {
    Node *pointer = node;
    while (pointer->right) pointer = pointer->right;
    return pointer;
}
