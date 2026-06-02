#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType;
typedef unsigned long InfoType;
typedef unsigned long VerType;

typedef struct Stack{
    int size; 
    int top;
    KeyType *data;
}Stack;

typedef struct Node{
    KeyType key;
    InfoType info;
    struct Node *left;
    struct Node *right;
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
    node->left=node->right=NULL;
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

int delete(Tree* tree, KeyType key){ //хуйня ебаная- пиздос - deepseek в помощь
    if (tree == NULL || key == NULL || tree->root == NULL) return -1;
    
    Node** p_cur = &tree->root;  // Двойной указатель для изменения ссылок
    
    // Поиск узла
    while (*p_cur != NULL) {
        int cmp = compare(key, (*p_cur)->key);
        if (cmp == 0) break;  // Нашли
        if (cmp < 0) {
            p_cur = &(*p_cur)->left;
        } else {
            p_cur = &(*p_cur)->right;
        }
    }
    
    if (*p_cur == NULL) return -1;  // Не найден
    Node* to_del = *p_cur;
    
    // Случай: 0 или 1 ребёнок
    if (to_del->left == NULL || to_del->right == NULL) {
        Node* child = (to_del->left != NULL) ? to_del->left : to_del->right;
        *p_cur = child;  // Переподключаем родителя к ребёнку
        free(to_del->key);
        free(to_del);
    } 
    // Случай: 2 ребёнка
    else {
        // Находим преемника (минимум правого поддерева)
        Node** p_succ = &to_del->right;
        while ((*p_succ)->left != NULL) {
            p_succ = &(*p_succ)->left;
        }
        Node* succ = *p_succ;
        
        // Копируем данные преемника в удаляемый узел
        free(to_del->key);
        to_del->key = strdup(succ->key);
        to_del->info = succ->info;
        
        // Удаляем преемника (у него точно ≤1 ребёнок)
        *p_succ = succ->right;
        free(succ->key);
        free(succ);
    }
    
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
    Node* par=NULL;
    while (cur!=NULL){
        int cmp=compare(node->key, cur->key);
        if (cmp<0){
            par=cur;
            cur=cur->left;
        }else if(cmp>0){
            cur=cur->right;
        }else{
            break; 
        }
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
    Node* par=NULL;
    while (cur!=NULL){
        int cmp=compare(node->key, cur->key);
        if (cmp>0){
            par=cur;
            cur=cur->right;
        }else if(cmp<0){
            cur=cur->left;
        }else{
            break; 
        }
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