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

int delete(Tree *tree, Node *x) {
    if (!tree || !tree->root || !x) return -1;

    Node *y = NULL,
        *p = NULL,
        *par = NULL;

    if (!x->right || !x->left) y = x;
    else                       y = successor(x);
    
    if (y->left) p = y->left;
    else         p = y->right;

    par = y->parent;
    
    if (p) p->parent = par;
    
    if (!par) tree->root = p;
    else {
        if (par->left == y) par->left = p;
        else                par->right = p;
    }
    
    if (y != x) x->key = y->key;

    free(y);

    return 0;
}
