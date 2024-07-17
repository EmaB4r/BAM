#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "../dependencies/token.h"
#include <stdint.h>
#include "../dependencies/List/list.h"
#include "../dependencies/Errors/Errors.h"
#include <string.h>
typedef enum{
    op_push,
    op_pop,
    op_add,
    op_sub,
    op_mul,
    op_div,
    op_swap,
    op_roll,
    op_jmp,
    op_jeq,
    op_jne,
    op_jlt,
    op_jle,
    op_jgt,
    op_jge
}instruction_type;

typedef struct {
    instruction_type type;
    uint64_t operand;
    char* lable;
}basm_quasinstruction;

typedef struct {
    lexer_t lexer;
    token_t current_token;
    token_t previous_token;
    list_t instructions_list;
    int curr_instr_addr;
}parser_t;

parser_t parser_init(char*source_code_path);
void parser_parse(parser_t* parser);
void print_all_lables();
int get_lable_val(char* name);
#endif