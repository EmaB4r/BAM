#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "../dependencies/token.h"
#include <stdint.h>
#include "../dependencies/List/list.h"
#include "../dependencies/Errors/Errors.h"
#include <string.h>


typedef struct {
    lexer_t lexer;
    token_t current_token;
    token_t previous_token;
    list_t * instructions_list;
    int curr_instr_addr;
}parser_t;

//initializes a parser object
parser_t parser_init(char*source_code_path);

//function that parses a program 
void parser_parse(parser_t* parser);

void parser_free(parser_t* parser);

//prints all lables declared inside the basm program
void print_all_lables();

//given a lable name returns the address it represents
int get_lable_val(char* name);
#endif