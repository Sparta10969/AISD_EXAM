#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char* KeyType; 
typedef unsigned long InfoType;
typedef unsigned long IndexType;

typedef struct Space{
    KeyType key;
    InfoType info;
    struct Space *next;
}Space;

typedef struct Table{
    IndexType size;
    struct Space *head;
}Table;

Table* create(void){
    Table* table=(Table*)malloc(sizeof(Table));
    if (table==NULL) return NULL;
    table->size=0;
    table->head=NULL;
    return table;
}

int compare(KeyType key1,KeyType key2){
    return strcmp(key1,key2);
}

int find(Table *table, KeyType key){
    if (table==NULL || key==NULL) return -1;
    Space *cur=table->head;
    while (cur){
        if (compare(cur->key, key)==0){
            return 0;
        }
        cur=cur->next;
    }
    return -1;
}

int insert(Table *table, KeyType key, InfoType info){
    if (table==NULL || key==NULL) return -1;
    if (find(table, key)!=-1) return -1;
    Space *new=(Space*)malloc(sizeof(Space));
    if (new==NULL) return -1;
    new->info=info;
    new->key=strdup(key);
    new->next=table->head;
    table->head=new;
    table->size++;
    return 0;
}

int delete(Table *table, KeyType key, InfoType *output){
    if (table==NULL || key==NULL) return -1;
    Space **cur=&table->head;
    while (*cur){
        if (compare((*cur)->key, key)==0){
            Space* time=*cur;
            *output=time->info;
            *cur=time->next;
            free(time->key);
            free(time);
            table->size--;
            return 0;
        }
        cur=&(*cur)->next;
    }
    return -1;   
}



int delete(Table *table, KeyType key, InfoType *output){
    if (table==NULL || key==NULL) return -1;
    Space *cur=table->head;
    Space *prev=NULL;
    while (cur){
        if (compare(cur->key, key)==0){
            *output=cur->info;
            if (prev==NULL){
                table->head=cur->next;
            }else{
                prev->next=cur->next;
            }
            free(cur->key);
            free(cur);
            table->size--;
            return 0;
        }
        prev=cur;
        cur=cur->next;
    }
    return -1;   
}

void kill(Table *table){
    Space *cur=table->head;
    while (cur){
        Space* time=cur->next;
        free(cur->key);
        free(cur);
        cur=time;
    }
    free(table);
}