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

Node *min(Node *node) {
    Node *pointer = node;
    while (pointer) pointer = pointer->left;
    return pointer;
}

Node *max(Node *node) {
    Node *pointer = node;
    while (pointer) pointer = pointer->right;
    return pointer;
}

Node *successor(Node *node) {
    if (node->right) return min(node->right);

    Node *x = node;
    Node *parent = x->parent;

    while (parent && x == parent->right) {
        x = parent;
        parent = x->parent;
    }

    return parent;
}

Node *predecessor(Node *node) {
    if (node->left) return max(node->left);

    Node *x = node;
    Node *parent = x->parent;

    while (parent && x == parent->left) {
        x = parent;
        parent = x->parent;
    }

    return parent;
}
