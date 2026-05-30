#include <stdio.h>
#include <stdlib.h>

typedef struct Tree{
    struct Node* root;
}Tree;

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
} Node;

// =====================================================
// ЗАДАНИЕ 1: Обратный обход (левое - правое - корень)
// =====================================================

// Вспомогательная функция для обратного обхода
void postorder_to_list(Node* node, Node** head, Node** tail) {
    if (node == NULL) return;
    
    // 1. Левое поддерево
    postorder_to_list(node->left, head, tail);
    
    // 2. Правое поддерево
    postorder_to_list(node->right, head, tail);
    
    // 3. Корень - добавляем в список
    node->left = *tail;  // используем left как prev
    node->right = NULL;  // используем right как next
    
    if (*tail != NULL) {
        (*tail)->right = node;
    } else {
        *head = node;  // первый элемент
    }
    *tail = node;
}

// Основная функция преобразования (обратный обход)
void tree_to_postorder_list(Node* root, Node** head, Node** tail) {
    *head = NULL;
    *tail = NULL;
    postorder_to_list(root, head, tail);
}

// =====================================================
// ЗАДАНИЕ 2: Прямой обход (корень - левое - правое)
// =====================================================

// Вспомогательная функция для прямого обхода
void preorder_to_list(Node* node, Node** head, Node** tail) {
    if (node == NULL) return;
    
    // 1. Корень - добавляем в список
    node->left = *tail;  // используем left как prev
    node->right = NULL;  // используем right как next
    
    if (*tail != NULL) {
        (*tail)->right = node;
    } else {
        *head = node;  // первый элемент
    }
    *tail = node;
    
    // Сохраняем указатели на детей до рекурсии
    Node* left_child = node->left;
    Node* right_child = node->right;
    
    // 2. Левое поддерево
    preorder_to_list(left_child, head, tail);
    
    // 3. Правое поддерево
    preorder_to_list(right_child, head, tail);
}

// Основная функция преобразования (прямой обход)
void tree_to_preorder_list(Node* root, Node** head, Node** tail) {
    *head = NULL;
    *tail = NULL;
    preorder_to_list(root, head, tail);
}

// =====================================================
// Вспомогательные функции для тестирования
// =====================================================

// Создание нового узла
Node* create_node(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Вставка в дерево поиска
Node* insert(Node* node, int key) {
    if (node == NULL) {
        return create_node(key);
    }
    
    if (key < node->key) {
        node->left = insert(node->left, key);
    } else {
        node->right = insert(node->right, key);
    }
    return node;
}

// Печать двусвязного списка
void print_list(Node* head) {
    Node* current = head;
    printf("Список: ");
    while (current != NULL) {
        printf("%d ", current->key);
        current = current->right;  // next
    }
    printf("\n");
}

// Пример использования
int main() {
    // Создаем дерево:
    //       50
    //      /  \
    //    30    70
    //   /  \   / \
    //  20  40 60 80
    
    Node* root = NULL;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 70);
    insert(root, 20);
    insert(root, 40);
    insert(root, 60);
    insert(root, 80);
    
    Node* head1 = NULL;
    Node* tail1 = NULL;
    
    // ЗАДАНИЕ 1: Обратный обход
    printf("=== ЗАДАНИЕ 1 ===\n");
    printf("Обратный обход (левое-правое-корень):\n");
    tree_to_postorder_list(root, &head1, &tail1);
    print_list(head1);
    // Ожидаемый порядок: 20 40 30 60 80 70 50
    
    // Пересоздаем дерево для второго задания
    root = NULL;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 70);
    insert(root, 20);
    insert(root, 40);
    insert(root, 60);
    insert(root, 80);
    
    Node* head2 = NULL;
    Node* tail2 = NULL;
    
    // ЗАДАНИЕ 2: Прямой обход
    printf("\n=== ЗАДАНИЕ 2 ===\n");
    printf("Прямой обход (корень-левое-правое):\n");
    tree_to_preorder_list(root, &head2, &tail2);
    print_list(head2);
    // Ожидаемый порядок: 50 30 20 40 70 60 80
    
    return 0;
}