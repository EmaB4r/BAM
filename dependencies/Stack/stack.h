#ifndef STACK_H
#define STACK_H
#include <stdint.h>
#include <stddef.h>

//used to store on the stack the size of the item below
#define PADDING sizeof(size_t)

//given two pointers (supposed same memory size allocated) and the allocated mem size
//copies bytes values from SRCPTR to DESTPTR
#define BYTECOPY(DESTPTR, SRCPTR, SRCSIZE)        \
    do{                                     \
        for (int I =0; I<SRCSIZE; I++){     \
            ((char*)DESTPTR)[I]=((char*)SRCPTR)[I];           \
        }                                   \
    }while(0)



typedef struct stack_s{
    uint8_t* data;
    unsigned stack_size;
    unsigned used_space;
}*stack_t;

//saves <item_size> bytes starting from pointer item onto the GLOBAL stack  
void stack_push(void * item, size_t item_size);

//pops first item from the GLOBAL stack and returns a pointer to it
// #CAST NEEDED# like type var = *(type*)stack_pop(); 
void* stack_pop();

void stack_print(char* str);

//frees GLOBAL stack obj
void stack_free();

//initializes a stack obj
stack_t sstack_init(unsigned size);

//saves <item_size> bytes starting from pointer item onto the given stack  
void sstack_push(stack_t stack, void* item, size_t item_size);

//pops first item from the stack and returns a pointer to it
// #CAST NEEDED# like type var = *(type*)stack_pop(); 
void * sstack_pop(stack_t stack);

void sstack_print(stack_t stack, char* str);

//frees given stack
void sstack_free(stack_t  stack);


//pushes an item variable onto the GLOBAL stack
#define STACK_PUSH(ITEM) stack_push(&ITEM, sizeof(ITEM))

#define STACK_PUSH_NUM(ITEM_TYPE, ITEM) do{ITEM_TYPE VAR = ITEM; STACK_PUSH(VAR);}while(0)

//pushes an item variable onto the given stack
#define SSTACK_PUSH(STACK,ITEM) sstack_push(STACK, &ITEM, sizeof(ITEM))

//pops a value from the GLOBAL stack and casts it to TYPE
#define STACK_POP(TYPE) (*(TYPE*)stack_pop())

//pops a value from the given stack and casts it to TYPE
#define SSTACK_POP(STACK, TYPE) (*(TYPE*)sstack_pop(STACK))




#endif