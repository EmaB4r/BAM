#include "token.h"


char *token_types[]={
    "token_end",
    
    //instructions
    //with parameter
    "token_push",
    "token_roll",
    "token_jmp",
    "token_jeq",
    "token_jne",
    "token_jlt",
    "token_jgt",
    "token_jle",
    "token_jge",
    
    //instructions
    //without parameter
    "token_pop",
    "token_dup",
    "token_swap",
    "token_add",
    "token_sub",
    "token_mul",
    "token_div",
    
    //metainstructions
    "token_lable_def",
    "token_lable",
    "token_precompiler",
    "token_num"
};

token_t token_init(token_type type, char *str_val, uint64_t num_val){
    token_t t = malloc(sizeof(*t));
    t->type=type;
    t->num_val=num_val;
    t->str_val=str_val;
    return t;
}

void token_free(token_t token){
    if(token->str_val!=NULL) free(token->str_val);
    free(token);
}

void token_printf(token_t token){
    printf("%s %lu  %s\n", token->str_val, token->num_val, token_types[token->type]);
}

int instr_with_param(token_type type){
    return type >= token_push && type <= token_jge;
}
int instr_without_param(token_type type){
    return type >= token_pop && type <= token_div;
}