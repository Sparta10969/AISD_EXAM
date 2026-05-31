#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType;
typedef unsigned long InfoType;

const int T = 2;  // степень дерева (2-3-4 дерево)

typedef struct Node {
    int number;  // текущее количество ключей (1..3)
    KeyType key[2*T - 1];      // ключи: key[0]..key[n-1]
    InfoType info[2*T - 1];    // сопутствующая информация
    struct Node* child[2*T];     // указатели на потомков: ptr[0]..ptr[n]
    struct Node* par;          // указатель на родителя
} Node;

typedef struct Tree {
    struct Node* root;
} Tree;

Node EList = { 0, {NULL}, {0}, {NULL}, NULL };

/* Сравнение ключей */
int compare(KeyType a, KeyType b) {
    return strcmp(a, b);
}

/* Поиск позиции: возвращает i, где key[i-1] < k <= key[i] */
int find_pos(Node* node, KeyType key) {
    int i = 0;
    while (i < node->number && compare(key, node->key[i]) > 0) {
        i++;
    }
    return i;
}

/* Проверка: узел заполнен (number == 2T-1 = 3) */
int is_full(Node* node) {
    if (node == &EList) return 0;
    return (node->number == 2*T - 1);
}

/* Проверка: узел минимален (number == T-1 = 1) */
int is_minimal(Node* node) {
    if (node == &EList) return 0;
    return (node->number == T - 1);
}

/* Разбиение полного узла y = x->child[i] */
void B_Tree_Split(Tree* tree, Node* x, int i) {
    Node* y = x->child[i];  /* полный узел: 3 ключа */
    
    /* Создаём новый узел z */
    Node* z = (Node*)malloc(sizeof(Node));
    z->number = T - 1;  /* 1 ключ */
    z->par = x;
    
    /* Копируем в z ключи и info из правой половины y */
    int j;
    for (j = 0; j < T - 1; j++) {
        z->key[j] = y->key[j + T];
        z->info[j] = y->info[j + T];
    }
    
    /* Копируем указатели потомков */
    if (y->child[0] != &EList) {
        for (j = 0; j < T; j++) {
            z->child[j] = y->child[j + T];
            if (z->child[j] != &EList) {
                z->child[j]->par = z;
            }
        }
    } else {
        for (j = 0; j < T; j++) {
            z->child[j] = &EList;
        }
    }
    
    /* Уменьшаем количество ключей в y */
    y->number = T - 1;
    
    /* Сдвигаем ключи и указатели в родителе x, начиная с позиции i */
    for (j = x->number; j > i; j--) {
        x->key[j] = x->key[j - 1];
        x->info[j] = x->info[j - 1];
        x->child[j + 1] = x->child[j];
        if (x->child[j + 1] != &EList) {
            x->child[j + 1]->par = x;
        }
    }
    
    /* Вставляем медианный ключ из y в позицию i родителя */
    x->key[i] = y->key[T - 1];
    x->info[i] = y->info[T - 1];
    x->child[i + 1] = z;
    x->number++;
    
    /* Обновляем родителя у оставшихся указателей y */
    for (j = 0; j <= y->number; j++) {
        if (y->child[j] != &EList) {
            y->child[j]->par = y;
        }
    }
}

/* Вставка в незаполненный узел */
void B_Insert_Nonfull(Tree* tree, Node* x, KeyType key, InfoType info) {
    int i;
    
    /* Если x — лист */
    if (x->child[0] == &EList) {
        /* Сдвигаем ключи для вставки в упорядоченном виде */
        i = x->number - 1;
        while (i >= 0 && compare(key, x->key[i]) < 0) {
            x->key[i + 1] = x->key[i];
            x->info[i + 1] = x->info[i];
            i--;
        }
        x->key[i + 1] = key;
        x->info[i + 1] = info;
        x->number++;
        return;
    }
    
    /* Поиск поддерева для спуска */
    i = find_pos(x, key);
    
    /* Если дубликат — не вставляем */
    if (i < x->number && compare(key, x->key[i]) == 0) {
        return;
    }
    
    /* Если дочерний узел заполнен — разбиваем */
    if (is_full(x->child[i])) {
        B_Tree_Split(tree, x, i);
        /* После разбиения выбираем нужное поддерево */
        if (compare(key, x->key[i]) > 0) {
            i++;
        }
    }
    
    /* Рекурсивный спуск */
    B_Insert_Nonfull(tree, x->child[i], key, info);
}

/* Основная функция вставки */
int correct_insert(Tree* tree, KeyType key, InfoType info) {
    if (tree == NULL || key == NULL) return -1;
    
    /* Если дерево пустое — создаём корень-лист */
    if (tree->root == NULL) {
        tree->root = (Node*)malloc(sizeof(Node));
        tree->root->number = 0;
        tree->root->par = NULL;
        tree->root->child[0] = &EList;
    }
    
    /* Если корень заполнен — создаём новый корень */
    if (is_full(tree->root)) {
        Node* s = (Node*)malloc(sizeof(Node));
        s->number = 0;
        s->par = NULL;
        s->child[0] = tree->root;
        tree->root->par = s;
        tree->root = s;
        B_Tree_Split(tree, s, 0);
    }
    
    /* Вставка в незаполненный корень */
    B_Insert_Nonfull(tree, tree->root, key, info);
    return 0;
}

/* Заимствование у левого соседа */
void borrow_from_left(Tree* tree, Node* parent, int i, Node* deficient) {
    Node* left = parent->child[i - 1];
    
    /* Сдвигаем ключи в deficient вправо */
    int j;
    for (j = deficient->number; j > 0; j--) {
        deficient->key[j] = deficient->key[j - 1];
        deficient->info[j] = deficient->info[j - 1];
        deficient->child[j + 1] = deficient->child[j];
    }
    deficient->child[1] = deficient->child[0];
    
    /* Берём разделитель из родителя */
    deficient->key[0] = parent->key[i - 1];
    deficient->info[0] = parent->info[i - 1];
    
    /* Если не лист — берём указатель из left */
    if (left->child[0] != &EList) {
        deficient->child[0] = left->child[left->number];
        if (deficient->child[0] != &EList) {
            deficient->child[0]->par = deficient;
        }
    }
    
    /* Обновляем разделитель в родителе */
    parent->key[i - 1] = left->key[left->number - 1];
    parent->info[i - 1] = left->info[left->number - 1];
    
    left->number--;
    deficient->number++;
}

/* Заимствование у правого соседа */
void borrow_from_right(Tree* tree, Node* parent, int i, Node* deficient) {
    Node* right = parent->child[i + 1];
    
    /* Берём разделитель из родителя */
    deficient->key[deficient->number] = parent->key[i];
    deficient->info[deficient->number] = parent->info[i];
    
    /* Если не лист — берём указатель из right */
    if (right->child[0] != &EList) {
        deficient->child[deficient->number + 1] = right->child[0];
        if (deficient->child[deficient->number + 1] != &EList) {
            deficient->child[deficient->number + 1]->par = deficient;
        }
        /* Сдвигаем указатели в right */
        int j;
        for (j = 0; j < right->number; j++) {
            right->child[j] = right->child[j + 1];
        }
    }
    
    /* Обновляем разделитель в родителе */
    parent->key[i] = right->key[0];
    parent->info[i] = right->info[0];
    
    /* Сдвигаем ключи в right */
    for (int j = 0; j < right->number - 1; j++) {
        right->key[j] = right->key[j + 1];
        right->info[j] = right->info[j + 1];
    }
    
    deficient->number++;
    right->number--;
}

/* Слияние с левым соседом */
void merge_with_left(Tree* tree, Node* parent, int i, Node* deficient) {
    Node* left = parent->child[i - 1];
    
    /* Копируем разделитель из родителя в конец left */
    left->key[left->number] = parent->key[i - 1];
    left->info[left->number] = parent->info[i - 1];
    left->number++;
    
    /* Копируем ключи и указатели из deficient в left */
    int j;
    for (j = 0; j < deficient->number; j++) {
        left->key[left->number] = deficient->key[j];
        left->info[left->number] = deficient->info[j];
        left->number++;
    }
    if (deficient->child[0] != &EList) {
        for (j = 0; j <= deficient->number - (T - 1); j++) {
            left->child[left->number - deficient->number + j] = deficient->child[j];
            if (left->child[left->number - deficient->number + j] != &EList) {
                left->child[left->number - deficient->number + j]->par = left;
            }
        }
    }
    
    /* Удаляем разделитель из родителя */
    for (j = i - 1; j < parent->number - 1; j++) {
        parent->key[j] = parent->key[j + 1];
        parent->info[j] = parent->info[j + 1];
        parent->child[j + 1] = parent->child[j + 2];
        if (parent->child[j + 1] != &EList) {
            parent->child[j + 1]->par = parent;
        }
    }
    parent->number--;
    
    free(deficient);
}

/* Слияние с правым соседом */
void merge_with_right(Tree* tree, Node* parent, int i, Node* deficient) {
    Node* right = parent->child[i + 1];
    
    /* Сдвигаем ключи right вправо */
    int j;
    for (j = right->number; j > 0; j--) {
        right->key[j] = right->key[j - 1];
        right->info[j] = right->info[j - 1];
        right->child[j + 1] = right->child[j];
    }
    
    /* Вставляем разделитель */
    right->key[0] = parent->key[i];
    right->info[0] = parent->info[i];
    right->number++;
    
    /* Копируем ключи и указатели из deficient */
    for (j = 0; j < deficient->number; j++) {
        right->key[right->number] = deficient->key[j];
        right->info[right->number] = deficient->info[j];
        right->number++;
    }
    if (deficient->child[0] != &EList) {
        for (j = 0; j <= deficient->number; j++) {
            right->child[j] = deficient->child[j];
            if (right->child[j] != &EList) {
                right->child[j]->par = right;
            }
        }
    }
    
    /* Удаляем разделитель из родителя */
    for (j = i; j < parent->number - 1; j++) {
        parent->key[j] = parent->key[j + 1];
        parent->info[j] = parent->info[j + 1];
        parent->child[j + 1] = parent->child[j + 2];
        if (parent->child[j + 1] != &EList) {
            parent->child[j + 1]->par = parent;
        }
    }
    parent->number--;
    
    free(deficient);
}

/* Удаление ключа из листа */
void delete_from_leaf(Node* leaf, int pos) {
    int i;
    for (i = pos; i < leaf->number - 1; i++) {
        leaf->key[i] = leaf->key[i + 1];
        leaf->info[i] = leaf->info[i + 1];
    }
    leaf->number--;
}

/* Удаление минимального ключа в поддереве */
void delete_min(Tree* tree, Node* node, KeyType* out_key, InfoType* out_info) {
    while (node->child[0] != &EList) {
        if (is_minimal(node->child[0])) {
            if (node->child[1] != &EList && !is_minimal(node->child[1])) {
                borrow_from_left(tree, node, 0, node->child[0]);
            } else if (node->child[1] != &EList) {
                merge_with_left(tree, node, 1, node->child[0]);
                node = node->child[0];
                continue;
            }
        }
        node = node->child[0];
    }
    *out_key = node->key[0];
    *out_info = node->info[0];
    delete_from_leaf(node, 0);
}

/* Удаление максимального ключа в поддереве */
void delete_max(Tree* tree, Node* node, KeyType* out_key, InfoType* out_info) {
    while (node->child[node->number] != &EList) {
        int last = node->number;
        if (is_minimal(node->child[last])) {
            if (last > 0 && node->child[last - 1] != &EList && !is_minimal(node->child[last - 1])) {
                borrow_from_right(tree, node, last - 1, node->child[last]);
            } else if (last > 0) {
                merge_with_right(tree, node, last - 1, node->child[last]);
                node = node->child[last - 1];
                continue;
            }
        }
        node = node->child[last];
    }
    *out_key = node->key[node->number - 1];
    *out_info = node->info[node->number - 1];
    delete_from_leaf(node, node->number - 1);
}

/* Рекурсивное удаление */
void B_Delete(Tree* tree, Node* x, KeyType key) {
    int i = find_pos(x, key);
    
    /* Если ключ найден в текущем узле */
    if (i < x->number && compare(key, x->key[i]) == 0) {
        /* Если x — лист */
        if (x->child[0] == &EList) {
            delete_from_leaf(x, i);
            return;
        }
        
        /* Если левый потомок имеет достаточно ключей */
        if (x->child[i]->number >= T) {
            KeyType pred_key;
            InfoType pred_info;
            delete_max(tree, x->child[i], &pred_key, &pred_info);
            x->key[i] = pred_key;
            x->info[i] = pred_info;
        }
        /* Иначе используем правого потомка */
        else {
            KeyType succ_key;
            InfoType succ_info;
            delete_min(tree, x->child[i + 1], &succ_key, &succ_info);
            x->key[i] = succ_key;
            x->info[i] = succ_info;
        }
        return;
    }
    
    /* Ключ не в текущем узле — спускаемся */
    if (x->child[0] == &EList) {
        return;  /* не найдено */
    }
    
    /* Коррекция перед спуском, если дочерний узел минимален */
    if (is_minimal(x->child[i])) {
        /* Пробуем занять у левого соседа */
        if (i > 0 && x->child[i - 1] != &EList && x->child[i - 1]->number >= T) {
            borrow_from_right(tree, x, i - 1, x->child[i]);
        }
        /* Пробуем занять у правого соседа */
        else if (i < x->number && x->child[i + 1] != &EList && x->child[i + 1]->number >= T) {
            borrow_from_left(tree, x, i, x->child[i]);
        }
        /* Слияние */
        else {
            if (i > 0) {
                merge_with_left(tree, x, i, x->child[i]);
                i--;
            } else {
                merge_with_right(tree, x, i, x->child[i]);
            }
        }
    }
    
    B_Delete(tree, x->child[i], key);
}

/* Основная функция удаления */
int correct_delete(Tree* tree, KeyType key) {
    if (tree == NULL || key == NULL || tree->root == NULL) return -1;
    
    B_Delete(tree, tree->root, key);
    
    /* Если корень стал пустым и имеет потомка — делаем его новым корнем */
    if (tree->root->number == 0) {
        if (tree->root->child[0] != &EList) {
            Node* old_root = tree->root;
            tree->root = tree->root->child[0];
            tree->root->par = NULL;
            free(old_root);
        }
    }
    
    return 0;
}

void free_btree(Node* node) {
    if (node == NULL || node == &EList) return;
    for (int i = 0; i <= node->number; i++) {
        free_btree(node->child[i]);
    }
    free(node);
}

void destroy_tree(Tree* tree) {
    if (tree == NULL) return;
    free_btree(tree->root);
    free(tree);
}