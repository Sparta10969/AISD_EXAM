#include <stdio.h>
#include <stdlib.h>

typedef char* KeyType; 
typedef unsigned long InfoType;
typedef unsigned long IndexType;

typedef struct Space{
    KeyType key;
    InfoType *info;
    struct Space* next;
}Space;

typedef struct Table{
    IndexType size;
    struct Space *head;
}Table;

Table* create(void){
    Table* table=(Table*)malloc(sizeof(Table));
    table->size=0;
    table->head=NULL;
    return table;
}