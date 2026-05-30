#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType; 
typedef unsigned long InfoType;
typedef unsigned long IndexType;

typedef struct Table{
    IndexType size;
    IndexType capacity;
    InfoType *info;
    KeyType *key;
    IndexType *busy;
}Table;

int compare(KeyType key1, KeyType key2){
    return strcmp(key1, key2);
}

Table* create(int capacity){
    Table* table=(Table*)malloc(sizeof(Table));
    table->capacity=capacity;
    table->size=0;
    table->busy=(IndexType*)malloc(sizeof(IndexType)*capacity);
    for (int i=0; i<capacity; i++) table->busy[i]==0; 
    table->key=(KeyType*)malloc(sizeof(KeyType)*capacity);
    table->info=(InfoType*)malloc(sizeof(InfoType)*capacity);
    return table;
}

int musor(Table *table){
    int i=0; int j=0;
    for (i; i<table->size; i++){
        if (table->busy[i]==1){
            table->key[j]=table->key[i];
            table->info[j]=table->info[i];
            table->busy[j]=1;
            j++;
        }
    }
    table->size=j;
    return 0;
}

int insert(Table *table, KeyType key, InfoType info){
    if (table==NULL || key==NULL || table->size>=table->capacity) return -1;
    int i=find(table,key);
    if (i!=-1 || table->busy[i]==1) return -1;
    if (table->size>=table->capacity){
        musor(table);
        if (table->size>=table->capacity) return -1;
    }
    table->key[table->size]=strdup(key);
    table->info[table->size]=info;
    table->busy[table->size]=1;
    table->size++;
    return 0;
}

int output(Table *table, KeyType key ,InfoType *output){
    if (table==NULL) return -1;
    int id=find(table, key);
    if (id==-1) return -1;
    *output=table->info[id];
    return 0;

}

int find(Table *table, KeyType key){
    if (table==NULL || key==NULL) return -1;
    for (int i=0; i<table->size; i++){
        if (compare(table->key[i],key)==0 && table->busy[i]==1) return i;
    }
    return -1;
}

void kill(Table *table){
    for (int i; i<table->size; i++){
        free(table->key[i]);
    }
    free(table->key);
    free(table->info);
    free(table->busy);
    free(table);
}