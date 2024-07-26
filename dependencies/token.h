#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum{
    token_end,

//instructions
//with parameter
    token_push,
    token_roll,
    token_jsr,
    token_jmp,
    token_jeq,
    token_jne,
    token_jlt,
    token_jgt,
    token_jle,
    token_jge,

//instructions
//without parameter
    token_pop,
    token_dup,
    token_stop,
    token_rts,
    token_print,
    token_print_stack,
    token_swap,
    token_add,
    token_sub,
    token_mul,
    token_div,

    //metainstructions
    token_lable_def,
    token_lable,
    token_precompiler,
    token_num
}token_type;

int instr_with_param(token_type type);
int instr_without_param(token_type type);


typedef struct{
    token_type type;
    char* str_val;
    uint64_t num_val;
}*token_t;

token_t token_init(token_type type, char* str_val, uint64_t num_val);
void token_free(token_t token);
void token_printf(token_t token);
char* get_token_name(token_type type);
#endif
