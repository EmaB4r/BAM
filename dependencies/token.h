#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// Define the list of tokens with X() wrapping each token
#define OPS_LIST \
    X(token_end) \
    /* Instructions with parameter */ \
    X(token_push) \
    X(token_roll) \
    X(token_dup) \
    X(token_jsr) \
    X(token_andi) \
    X(token_ori) \
    X(token_noti) \
    X(token_shli) \
    X(token_shri) \
    X(token_addi) \
    X(token_subi) \
    X(token_muli) \
    X(token_divi) \
    X(token_jmp) \
    X(token_jeq) \
    X(token_jne) \
    X(token_jlt) \
    X(token_jgt) \
    X(token_jle) \
    X(token_jge) \
    /* Instructions without parameter */ \
    X(token_pop) \
    X(token_lb) \
    X(token_sb) \
    X(token_stop) \
    X(token_rts) \
    X(token_swap) \
    X(token_syscall) \
    X(token_print_stack) \
    X(token_and) \
    X(token_or) \
    X(token_not) \
    X(token_shl) \
    X(token_shr) \
    X(token_add) \
    X(token_sub) \
    X(token_mul) \
    X(token_div) \
    /* Metainstructions */ \
    X(token_lable_def) \
    X(token_lable) \
    X(token_precompiler_include) \
    X(token_precompiler_def) \
    X(token_precompiler_macro_def) \
    X(token_precompiler_end_macro_def) \
    X(token_precompiler_asciiz) \
    X(token_precompiler_byte) \
    X(token_str) \
    X(token_num)



typedef enum{
    #define X(op) op,
    OPS_LIST
    #undef X
}token_type;

int instr_with_param(token_type type);
int instr_without_param(token_type type);


typedef struct{
    token_type type;
    char* str_val;
    uint64_t num_val;
    int line;
}token_t;

void print_type(token_type type);
token_t token_init(token_type type, char* str_val, uint64_t num_val, int line);
void token_free(token_t * token);
void token_printf(void * token);
char* get_token_name(token_type type);
#endif
