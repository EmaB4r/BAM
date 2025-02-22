#include "list.h"
#include <stdio.h>
#include <string.h>

int list_is_empty(list_t * list){
    return !list->n_elements;
}

list_t * list_init(){
    list_t * list=malloc(sizeof(*list));
    list->head=list->tail=NULL;
    list->n_elements=0;
    return list;
}

node_t * node_init(void*item, size_t item_size){
    node_t * node=malloc(sizeof(struct node_s));
    node->item=malloc(item_size);
    node->item_size=item_size;
    node->prev=node->next=NULL;
    memcpy(node->item, item, item_size);
    return node;
}

void list_ins_tail(list_t * list, void* item, size_t item_size){
    if(list_is_empty(list)){
        list->head=list->tail=node_init(item, item_size);
    }
    else{
        list->tail->next=node_init(item, item_size);
        list->tail->next->prev=list->tail;
        list->tail=list->tail->next;
    }
    list->n_elements++;
}

void list_ins_head(list_t * list, void* item, size_t item_size){
    if(list_is_empty(list)){
        list->head=list->tail=node_init(item, item_size);
    }
    else{
        list->head->prev=node_init(item, item_size);
        list->head->prev->next=list->head;
        list->head=list->head->prev;
    }
    list->n_elements++;
}

void list_append_head(list_t*dest, list_t*src){
    node_t * tail=src->tail;
    while (tail!=NULL){
        list_ins_tail(dest, tail->item, tail->item_size);
        tail=tail->prev;
    }
}

void list_append(list_t*dest, list_t*src){
    node_t * head=src->head;
    while (head!=NULL){
        list_ins_tail(dest, head->item, head->item_size);
        head=head->next;
    }
}

void list_print(list_t*list, void(*printfun)(void*item)){
    node_t * head=list->head;
    while(head!=NULL){
        printfun(head->item);
        head=head->next;
    }
}

void list_free_R(node_t * head){
    if (head == NULL) return;
    list_free_R(head->next);
    free(head->item);
    free(head);
}


void list_free(list_t * list){
    list_free_R(list->head);
    free(list);
}