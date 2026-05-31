#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType;
typedef unsigned long InfoType;
typedef unsigned long VerType;

typedef struct Stack{
    int size; 
    int top;
    Node **data;
}Stack;

typedef struct Node{
    KeyType key;
    InfoType info;
    struct Node *left;
    struct Node *right;
    struct Node *par;
}Node;

typedef struct Tree{
    struct Node* root;
}Tree;

Stack* create_stack(void){
    Stack* stack=(Stack*)malloc(sizeof(Stack));
    stack->size=1;
    stack->top=0;
    return stack;
}

Tree* create(void){
    Tree* tree=(Tree*)malloc(sizeof(Tree));
    if (tree==NULL) return -1;
    tree->root=NULL;
    return tree;
}

int compare(KeyType key1, KeyType key2){
    return strcmp(key1, key2);
}

int insert(Tree *tree, KeyType key, InfoType info){
    if (tree==NULL || key==NULL) return -1;
    Node *node=(Node*)malloc(sizeof(Node));
    node->left=node->right=node->par=NULL;
    node->key=strdup(key);
    if (node->key==NULL) return -1;
    node->info=info;
    if (tree->root==NULL){
        tree->root=node;
        return 0;
    }
    Node *cur=tree->root;
    Node* par=NULL;
    while (cur){
        par=cur;
        if (compare(key, cur->key)<0){
            cur=cur->left;
        }else{
            cur=cur->right;
        }
    }
    node->par=par;
    if (compare(key, par->key)<0){
        par->left=node;
    }else{
       par->right=node;
    }
    return 0;
}

int search(Tree *tree, KeyType key, Node **output){
    if (tree==NULL || key==NULL) return -1;
    if (tree->root==NULL){
        *output=NULL;
        return 0;
    }
    Node *cur=tree->root;
    Node *par=NULL;
    while (cur!=NULL){
        par=cur;
        if (compare(key, cur->key)==0){
            *output=cur;
            return 0;
        }
        if (compare(key, cur->key)<0){
            cur=cur->left;
        }else{
            cur=cur->right;
        }
    }
    return -1;
}

int delete(Tree* tree, KeyType key){
    if (tree=NULL || key==NULL || tree->root==NULL) return -1;
    Node *rem;
    Node *node;
    int i=search(tree, key, &node);
    if (i==-1) return -1;
    if (node==NULL) return -1;
    Node *real_del;
    if (node->left==NULL || node->right==NULL){
        real_del=node;
    }else{
        int i=successor(tree, node, &real_del);
        if (i==-1) return -1;
    }
    Node *child;
    if (real_del!=NULL) child=real_del->left;
    else child=real_del->right;
    if (child!=NULL) child->par=real_del->par;
    if (real_del->par==NULL) {
        tree->root=child; 
    } else {
        if (real_del==real_del->par->left) {
            real_del->par->left=child;
        } else {
            real_del->par->right=child;
        }
    }
    
    if (real_del!=node) {
        free(node->key);
        node->key=strdup(real_del->key);
        node->info=real_del->info;
    }
    free(real_del->key);
    free(real_del);
    return 0;
}

int search_max(Tree *tree, Node **output){
    if (tree==NULL) return -1;
    Node *cur=tree->root;
    while (cur->right!=NULL){
        cur=cur->right;
    }
    *output=cur;
    return 0;
}

int search_min(Tree *tree, Node **output){
    if (tree==NULL) return -1;
    Node *cur=tree->root;
    while (cur->left!=NULL){
        cur=cur->left;
    }
    *output=cur;
    return 0;
}

int successor(Tree *tree, Node *node, Node **output){ //следующий
    if (tree==NULL || node==NULL || output==NULL) return -1;
    if (node->right!=NULL){
        Node *cur=node->right;
        while (cur->left!=NULL){
            cur=cur->left;
        }
        *output=cur;
        return 0;
    }
    Node* cur=node;
    Node* par=cur->par;
    while (par!=NULL && cur==par->right){
        cur=par;
        par=cur->par;
    }
    if (par==NULL) return -1;
    *output=par;
    return 0;
}

int predecessor(Tree *tree, Node *node, Node **output){ //предшествующий
    if (tree==NULL || node==NULL || output==NULL) return -1;
    if (node->left!=NULL){
        Node* cur=node->left;
        while (cur->right!=NULL){
            cur=cur->right;
        }
        *output=cur;
        return 0;
    }
    Node *cur=node;
    Node *par=cur->par;
    while (par!=NULL && cur==par->left){
        cur=par;
        par=cur->par;
    }
    if (par==NULL) return -1;
    *output=par;
    return 0;
}

int pre_order_tree(Tree* tree){
    if (tree==NULL || tree->root==NULL) return -1;
    return pre_order(tree->root);
}

int pre_order(Node *node){ //прямой - корень - лево - право
    if (node==NULL) return -1;
    printf("%s", node->key);
    pre_order(node->left);
    pre_order(node->right);
    return 0;
}

int in_order_tree(Tree* tree){
    if (tree==NULL || tree->root==NULL) return -1;
    return in_order(tree->root);
}

int in_order(Node *node){ // центрированный - лево - корень - право
    if (node==NULL) return -1;
    in_order(node->left);
    printf("%s", node->key);
    in_order(node->right);
    return 0;
}

int post_order_tree(Tree* tree){
    if (tree==NULL || tree->root==NULL) return -1;
    return post_order(tree->root->left);
}

int post_order(Node *node){ //концевой - лево - право - корень
    if (node==NULL) return -1;
    post_order(node->left);
    post_order(node->right);
    printf("%s", node->key);
}

void post_order_free(Node* node) {
    if (node == NULL) return;
    post_order_free(node->left);
    post_order_free(node->right);
    free(node->key);
    free(node);
}

void kill_tree(Tree* tree) {
    if (tree == NULL) return;
    post_order_free(tree->root);
    free(tree);
}