#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType;
typedef unsigned long InfoType;
typedef unsigned long VerType;

typedef struct Node{
    KeyType key;
    InfoType info;
    struct Node *left;
    struct Node *right;
    struct Node *par;
    int balance;
}Node;

typedef struct Tree{
    struct Node* root;
}Tree;

Node EList={NULL,0,NULL,NULL,NULL,0};

Tree* create(void){
    Tree* tree=(Tree*)malloc(sizeof(Tree));
    if (tree==NULL) return -1;
    tree->root=NULL;
    return tree;
}

int compare(KeyType key1, KeyType key2){
    return strcmp(key1, key2);
}

int left_rotate(Tree *tree, Node* node){
    if (tree==NULL || node==NULL) return -1;
    Node *little_tree=node->right;
    if (little_tree==&EList) return -1;
    Node *parent=node->par;
    if (little_tree->left!=&EList){
        node->right=little_tree->left;
        little_tree->left->par=node;
    }
    little_tree->par=parent;
    if (parent==&EList){
        tree->root=little_tree;
    }else{
        if(parent->right==node){
            parent->right=little_tree;
        }else{
            parent->left=little_tree;
        }
    }
    little_tree->left=node;
    node->par=little_tree;
    return 0;
}

int right_rotate(Tree *tree, Node* node){
    if (tree==NULL || node==NULL) return -1;
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
        if(parent->right==node){
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
    /* Новый узел: баланс=0 */
    node->balance=0;
    Node *new=node;
    Node *parent=new->par;

    /* Поднимаемся вверх, пока баланс родителя==0 */
    while (parent!=NULL && parent!=&EList && parent->balance==0){
        if (new==parent->left){
            parent->balance=-1;
        } else {
            parent->balance=+1;
        }
        new=parent;
        parent=new->par;
    }
    /* Достигли корня — успех */
    if (parent==NULL || parent==&EList){
        return 0;
    }
    /* Вставка компенсировала наклон — баланс стал 0 */
    if (new==parent->left && parent->balance==+1){
        parent->balance=0;
        return 0;
    }
    if (new==parent->right && parent->balance==-1){
        parent->balance=0;
        return 0;
    }
    /* === Коррекция дерева (баланс стал ±2) === */
    /* Случай: вставка в левое поддерево */
    if (new==parent->left){
        Node *B=parent->right;
        /* Случай 1: одинарный поворот (LL) */
        if (B!=&EList && B->balance <= 0){
            left_rotate(tree, parent);
            if (B->balance==0){
                parent->balance=+1;
                B->balance=-1;
            } else {
                parent->balance=0;
                B->balance=0;
            }
        }
        /* Случай 2: двойной поворот (LR) */
        else if (B!=&EList){
            Node *C=B->left;
            int c_bal=0;
            if (C!=&EList){
                c_bal=C->balance;
            }
            right_rotate(tree, B);
            left_rotate(tree, parent);
            if (c_bal==-1){
                parent->balance=+1;
                B->balance=0;
            } else if (c_bal==+1){
                parent->balance=0;
                B->balance=-1;
            } else {
                parent->balance=0;
                B->balance=0;
            }
            if (C!=&EList){
                C->balance=0;
            }
        }
    }
    /* Случай: вставка в правое поддерево (симметрично) */
    else {
        Node *B=parent->left;
        
        if (B!=&EList && B->balance >= 0){
            right_rotate(tree, parent);
            if (B->balance==0){
                parent->balance=-1;
                B->balance=+1;
            } else {
                parent->balance=0;
                B->balance=0;
            }
        }
        else if (B!=&EList){
            Node *C=B->right;
            int c_bal=0;
            if (C!=&EList){
                c_bal=C->balance;
            }
            
            left_rotate(tree, B);
            right_rotate(tree, parent);
            
            if (c_bal==+1){
                parent->balance=-1;
                B->balance=0;
            } else if (c_bal==-1){
                parent->balance=0;
                B->balance=+1;
            } else {
                parent->balance=0;
                B->balance=0;
            }
            if (C!=&EList){
                C->balance=0;
            }
        }
    }
    
    return 0;
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
    correct_insert(tree,node);
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

/* === Фиксация после удаления === */
int correct_delete(Tree *tree, Node *new){
    if (tree==NULL || new==NULL) return -1;
    Node *parent=new->par;
    /* Поднимаемся вверх от удалённого узла */
    while (parent!=NULL && parent!=&EList){
        /* Обновляем баланс родителя */
        if (new==parent->left){
            parent->balance++;
        } else {
            parent->balance--;
        }
        int dh=parent->balance;
        if (dh < 0){
            dh=-dh;
        }
        /* Баланс восстановлен, но высота изменилась — продолжаем вверх */
        if (dh==1){
            new=parent;
            parent=new->par;
            continue;
        }
        /* Баланс стал 0 — высота не изменилась, коррекция завершена */
        if (dh==0){
            new=parent;
            parent=new->par;
            continue;
        }
        /* === Нарушение баланса (dh==2) — коррекция дерева === */
        /* Случай: левое поддерево тяжелее */
        if (parent->balance < 0){
            Node *B=parent->right;
            /* Случай 1: брат сбалансирован или внешнее поддерево выше */
            if (B!=&EList && B->balance >= 0){
                left_rotate(tree, parent);
                if (B->balance==0){
                    parent->balance=-1;
                    B->balance=+1;
                    new=parent;
                    parent=new->par;
                    continue;
                } else {
                    parent->balance=0;
                    B->balance=0;
                    break;
                }
            }
            /* Случай 2: брат наклонён внутрь */
            else if (B!=&EList){
                Node *C=B->left;
                int c_bal=0;
                if (C!=&EList){
                    c_bal=C->balance;
                }
                right_rotate(tree, B);
                left_rotate(tree, parent);
                if (c_bal==+1){
                    parent->balance=0;
                    B->balance=-1;
                } else if (c_bal==-1){
                    parent->balance=+1;
                    B->balance=0;
                } else {
                    parent->balance=0;
                    B->balance=0;
                }
                if (C!=&EList){
                    C->balance=0;
                }
                new=parent;
                parent=new->par;
                continue;
            }
        }
        /* Случай: правое поддерево тяжелее (симметрично) */
        else {
            Node *B=parent->left;
            if (B!=&EList && B->balance <= 0){
                right_rotate(tree, parent);
                if (B->balance==0){
                    parent->balance=+1;
                    B->balance=-1;
                    new=parent;
                    parent=new->par;
                    continue;
                } else {
                    parent->balance=0;
                    B->balance=0;
                    break;
                }
            }
            else if (B!=&EList){
                Node *C=B->right;
                int c_bal=0;
                if (C!=&EList){
                    c_bal=C->balance;
                }
                left_rotate(tree, B);
                right_rotate(tree, parent);
                if (c_bal==-1){
                    parent->balance=0;
                    B->balance=+1;
                } else if (c_bal==+1){
                    parent->balance=-1;
                    B->balance=0;
                } else {
                    parent->balance=0;
                    B->balance=0;
                }
                if (C!=&EList){
                    C->balance=0;
                }
                new=parent;
                parent=new->par;
                continue;
            }
        }
    }
    
    return 0;
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
    if (real_del->balance!=0 || child!=&EList) {
        correct_delete(tree, child);
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
    return 0;
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