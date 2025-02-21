#include "token.h"



char *token_types[]={
    #define X(op) #op,
    OPS_LIST
    #undef X
};

void print_type(token_type type){
    fputs(token_types[type], stdout);
}

token_t token_init(token_type type, char *str_val, uint64_t num_val, int line){
    token_t t = {.type=type,.str_val=str_val, .num_val=num_val, .line=line};
    return t;
}

void token_free(token_t * token){
    if(token->str_val!=NULL) free(token->str_val);
    free(token);
}

char* get_token_name(token_type type){
    return token_types[type];
}

int instr_with_param(token_type type){
    return type >= token_push && type <= token_jge;
}
int instr_without_param(token_type type){
    return type >= token_pop && type <= token_div;
}

void token_printf(void *token){
    printf("%s str: %s num %lu\n", token_types[((token_t*)token)->type], ((token_t*)token)->str_val, ((token_t*)token)->num_val);
}

