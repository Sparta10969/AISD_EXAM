#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType;
typedef unsigned long InfoType;
typedef unsigned long VerType;

typedef enum{
    RED,
    BLACK
}Color;

typedef struct Node{
    KeyType key;
    InfoType info;
    struct Node *left;
    struct Node *right;
    struct Node *par;
    Color color;
}Node;

typedef struct Tree{
    struct Node* root;
}Tree;

Node EList={NULL,0,NULL,NULL,NULL,NULL,BLACK};

Tree* create(void){
    Tree* tree=(Tree*)malloc(sizeof(Tree));
    if (tree==NULL) return -1;
    tree->root=NULL;
    return tree;
}

int compare(KeyType key1, KeyType key2){
    return strcmp(key1, key2);
}

int left_rotate(Tree *tree, Node *node){
    if (node==NULL) return -1;
    Node* little_tree=node->right;
    if (little_tree==&EList)return -1;
    Node * parent=node->par;
    node->right=little_tree->left;
    if (little_tree->left!=&EList){
        little_tree->left->par=node;
    }
    little_tree->par=parent;
    if (parent==&EList){
        tree->root=little_tree;
    }else{
        if (parent->left==node){
            parent->left=little_tree;
        }else{
            parent->right=little_tree;
        }
    }
    little_tree->left=node;
    node->par=little_tree;
    return 0;
}

int right_rotate(Tree *tree, Node * node){
    if (node==&EList) return -1;
    Node *little_tree=node->left;
    if (little_tree==&EList) return -1;
    Node *parent=node->par;
    if (little_tree->right!=&EList){
        node->left=little_tree->right;
        little_tree->right->par=node;
    }
    little_tree->par=parent;
    if (parent==&EList){
        tree->root=little_tree;
    }else{
        if (parent->right==node){
            parent->right=little_tree;
        }else{
            parent->left=little_tree;
        }
    }
    little_tree->right=node;
    node->par=little_tree;
    return 0;
}

int correct_insert(Tree *tree, Node *node){
    if (tree==NULL || node==NULL) return -1;
    while (node->par!=NULL && node->par->color==RED){
        /* Случай: родитель — левый ребёнок */
        if (node->par==node->par->par->left){
            Node *uncle=node->par->par->right;
            /* Случай 1: дядя красный → перекраска */
            if (uncle!=&EList && uncle->color==RED){
                node->par->color=BLACK;
                uncle->color=BLACK;
                node->par->par->color=RED;
                node=node->par->par;  /* подъём на два уровня */
            }
            /* Случай 2 и 3: дядя чёрный */
            else {
                /* Случай 2: (node — правый ребёнок) */
                if (node==node->par->right){
                    node=node->par;
                    left_rotate(tree,node);
                }
                /* Случай 3: линия + поворот + перекраска */
                node->par->color=BLACK;
                node->par->par->color=RED;
                right_rotate(tree, node->par->par);
            }
        }
        /* Симметрично: родитель — правый ребёнок */
        else {
            Node *uncle=node->par->par->left; 
            /* Случай 1: дядя красный → перекраска */
            if ( uncle!=&EList && uncle->color==RED){
                node->par->color=BLACK;
                uncle->color=BLACK;
                node->par->par->color=RED;
                node=node->par->par;
            }
            /* Случай 2 и 3: дядя чёрный */
            else {
                /* Случай 2: треугольник (node — левый ребёнок) */
                if (node==node->par->left){
                    node=node->par;
                    right_rotate(tree, node);
                }
                /* Случай 3: линия + поворот + перекраска */
                node->par->color=BLACK;
                node->par->par->color=RED;
                left_rotate(tree, node->par->par);
            }
        }
    }
    /* Корень всегда чёрный (свойство 2) */
    tree->root->color=BLACK;
    return 0;
}

int insert(Tree *tree, KeyType key, InfoType info){
    if (tree==NULL || key==NULL) return -1;
    Node *node=(Node*)malloc(sizeof(Node));
    node->left=node->right=node->par=NULL;
    node->key=strdup(key);
    if (node->key==NULL) return -1;
    node->info=info;
    node->color=RED;
    if (tree->root==NULL){
        tree->root=node;
        node->color=BLACK;
        return 0;
    }
    Node *cur=tree->root;
    Node* par=NULL;
    while (cur!=&EList){
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
    correct_insert_tree(tree, node);
    return 0;
}

int correct_delete(Tree *tree, Node *node){
    if (tree==NULL || node==NULL) return -1;
    /* Пока node не корень и node чёрный → нарушена чёрная высота */
    while (node!=tree->root && node->color==BLACK){
        /* === Случай: node — левый ребёнок === */
        if (node==node->par->left){
            Node *brother=node->par->right;
            /* Случай 1: брат красный → перекраска + поворот вокруг родителя */
            if (brother!=&EList && brother->color==RED){
                brother->color=BLACK;
                node->par->color=RED;
                left_rotate(tree, node->par);
                brother=node->par->right;
            }
            /* Случай 2: оба ребёнка брата чёрные → перекраска брата, подъём вверх */
            if ((brother->left==&EList || brother->left->color==BLACK) && 
                (brother->right==&EList || brother->right->color==BLACK)){
                brother->color=RED;
                node=node->par;
            }
            /* Случаи 3 и 4: хотя бы один ребёнок брата красный */
            else {
                /* Случай 3: дальний ребёнок брата чёрный, ближний красный */
                if (brother->right==&EList || brother->right->color==BLACK){
                    if (brother->left!=&EList) brother->left->color=BLACK;
                    brother->color=RED;
                    right_rotate(tree, brother);
                    brother=node->par->right;
                }
                /* Случай 4: дальний ребёнок брата красный → финальная коррекция */
                brother->color=node->par->color;
                node->par->color=BLACK;
                if (brother->right!=&EList) brother->right->color=BLACK;
                left_rotate(tree, node->par);
                node=tree->root;
            }
        }
        /* === Случай: node — правый ребёнок (симметрично) === */
        else {
            Node *brother=node->par->left;
            
            if (brother!=&EList && brother->color==RED){
                brother->color=BLACK;
                node->par->color=RED;
                right_rotate(tree, node->par);
                brother=node->par->left;
            }
            
            if ((brother->right==&EList || brother->right->color==BLACK) && 
                (brother->left==&EList || brother->left->color==BLACK)){
                brother->color=RED;
                node=node->par;
            }
            else {
                if (brother->left==&EList || brother->left->color==BLACK){
                    if (brother->right!=&EList) brother->right->color=BLACK;
                    brother->color=RED;
                    left_rotate(tree, brother);
                    brother=node->par->left;
                }
                brother->color=node->par->color;
                node->par->color=BLACK;
                if (brother->left!=&EList) brother->left->color=BLACK;
                right_rotate(tree, node->par);
                node=tree->root;
            }
        }
    }
    /* Корень всегда чёрный (или node стал красным → красим в чёрный) */
    node->color=BLACK;
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
    if (tree==NULL || key==NULL || tree->root==NULL) return -1;
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
    if (real_del->left!=NULL) child=real_del->left;
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
    correct_delete(tree,child);
    return 0;
}

int search_manode(Tree *tree, Node **output){
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
    if (node==NULL) return;
    post_order_free(node->left);
    post_order_free(node->right);
    free(node->key);
    free(node);
}

void kill_tree(Tree* tree) {
    if (tree==NULL) return;
    post_order_free(tree->root);
    free(tree);
}