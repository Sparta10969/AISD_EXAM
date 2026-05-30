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

void direct(Node *node) {
    if (node) {
        // обработка
        direct(node->left);
        direct(node->right);
    }
}

void symmetric(Node *node) {
    if (node) {
        symmetric(node->left);
        // обработка
        symmetric(node->right);
    }
}

void reverse(Node *node) {
    if (node) {
        reverse(node->left);
        reverse(node->right);
        // обработка
    }
}
