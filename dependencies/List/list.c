//
// Created by EmanueleBar on 05/12/2023.
//

#include "list.h"
#include <stdlib.h>

list_t list_init(){
    list_t l=(list_t)malloc(sizeof (struct list));
    l->tail=l->head=NULL;
    l->num_elem=0;
    return l;
}

void list_recursive_free(node_t nodo, void(*free_func)(void*)){
    if(nodo->next!=NULL){
        list_recursive_free(nodo->next, free_func);
        if(free_func!=NULL)
            free_func(nodo->Item);
        free(nodo);
    }
}

void list_free(list_t list, void(*free_func)(void*)){
    list_recursive_free(list->head, free_func);
    free(list);
}

void list_ins_head(list_t list, void* Item){
    if(list->head==NULL && list->tail==NULL){
        list->head=list->tail= node_init();
    }
    else{
        list->head->prev= node_init();
        list->head->prev->next=list->head;
        list->head=list->head->prev;
    }
    list->head->Item=Item;
    list->num_elem+=1;

}

void list_ins_tail(list_t list, void* Item){
    if(list->head==NULL && list->tail==NULL){
        list->head=list->tail= node_init();
    }
    else{
        list->tail->next= node_init();
        list->tail->next->prev=list->tail;
        list->tail=list->tail->next;
    }
    list->tail->Item=Item;
    list->num_elem+=1;
}

void list_concat_tail(list_t dest, list_t src){
    if(src->head==NULL && src->tail==NULL){
        free(src);
        return;
    }
    if(dest->head==NULL && dest->tail==NULL){
        *dest=*src;
        free(src);
        return;
    }
    dest->tail->next=src->head;
    src->head->prev=dest->tail;
    dest->tail=src->tail;
    free(src);
    
}


int list_get_num_elem(list_t list){
    return list->num_elem;
}

node_t node_init(){
    node_t nodo=(node_t) malloc(sizeof (struct node));
    nodo->next=NULL;
    nodo->prev=NULL;
    nodo->Item=NULL;
    return nodo;
}

node_t list_get_head(list_t list){
    return list->head;
}

node_t node_get_next(node_t node){
    return node->next;
}

node_t node_get_prev(node_t node){
    return node->prev;
}

void * node_get_val(node_t node){
    return node->Item;
}

void list_print(list_t l, void (printing_fun)(void *)){
    node_t n = list_get_head(l);
    while (n!=NULL)
    {
        printing_fun(node_get_val(n));
        n=node_get_next(n);
    }
}

void list_insertion_sort(list_t l, int(*item_gt)(void*, void*)){
    node_t n1=list_get_head(l);
    node_t min, tmp;
    void* tmp1;
    while (n1!=NULL){
        min=n1;
        tmp=node_get_next(n1);
        while(tmp!=NULL){
            if(item_gt(node_get_val(min), node_get_val(tmp))){
                min=tmp;
            }
            tmp= node_get_next(tmp);
        }
        tmp1=n1->Item;
        n1->Item=min->Item;
        min->Item=tmp1;

        n1=node_get_next(n1);
    }
}