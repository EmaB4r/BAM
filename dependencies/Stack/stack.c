#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

stack_t global_stack = NULL;


void stack_push(void * item, size_t item_size){
    if(global_stack==NULL){
        global_stack=sstack_init(100);
    }
    sstack_push(global_stack, item, item_size);
}

void* stack_pop(){
    if(global_stack==NULL){
        global_stack=sstack_init(100);
        return NULL;
    }
    return sstack_pop(global_stack);
}

void stack_print(char* str){
    if(global_stack!=0) sstack_print(global_stack, str);
}

void stack_free(){
    if(global_stack!=NULL) sstack_free(global_stack);
}


//stack_init
stack_t sstack_init(unsigned size){
    if(size<100) size = 100;
    stack_t s = malloc(sizeof(*s));
    s->used_space=0;
    s->stack_size=size;
    s->data=malloc(sizeof(uint8_t)*size);
    return s;
}

void sstack_push(stack_t stack, void* item, size_t item_size){
    if(stack->used_space+item_size + PADDING < stack->stack_size){
        stack->stack_size*=1.5;
        stack->data=realloc(stack->data, sizeof(uint8_t)*stack->stack_size);
    } 
    BYTECOPY(&stack->data[stack->used_space], item, item_size);
    stack->used_space+=item_size;
    *(size_t*)&stack->data[stack->used_space]=item_size;
    stack->used_space+=PADDING;
}
void * sstack_pop(stack_t stack){
    stack->used_space-=PADDING;
    size_t item_size = *(size_t*)&stack->data[stack->used_space];
    stack->used_space-=item_size;
    return &stack->data[stack->used_space];
}

void sstack_print(stack_t stack, char* str){
    int SP=stack->used_space-PADDING;
    size_t objsize;
    printf("--TOP--\n");
    while(SP>0){
        objsize = *(size_t*)&stack->data[SP];
        SP-=objsize;
        printf(str, *(size_t*)&stack->data[SP]);
        SP-=PADDING;
    }
    printf("--BOTTOM--\n");
}

void sstack_free(stack_t  stack){
    if (stack!=NULL){
        if(stack->data!=NULL){
            free(stack->data);
        }
        free(stack);
    }
}