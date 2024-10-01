#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdlib.h>

typedef struct node_s{
    struct node_s *prev;
    struct node_s *next;
    void * item;
    size_t item_size;
}node_t;

typedef struct list_s{
    node_t * head;
    node_t * tail;
    unsigned int n_elements;
}list_t;

list_t * list_init();

void list_ins_tail(list_t * list, void* item, size_t item_size);

void list_ins_head(list_t * list, void* item, size_t item_size);

void list_append(list_t*dest, list_t*src);

void list_print(list_t*list, void(*printfun)(void*item));

node_t * node_init(void * item, size_t item_size);

#define LIST_INS_TAIL(LIST, ITEM) list_ins_tail(LIST, &ITEM, sizeof(ITEM))
#define LIST_INS_HEAD(LIST, ITEM) list_ins_head(LIST, &ITEM, sizeof(ITEM))

void list_free(list_t * list);

#endif