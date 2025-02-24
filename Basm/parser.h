#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <string.h>
#include "lexer.h"
#include "lable.h"
#include "../dependencies/token.h"
#include "../dependencies/List/list.h"
#include "../dependencies/Errors/Errors.h"


typedef struct parser_s{
    lexer_t lexer;
    token_t current_token;
    token_t previous_token;
    list_t * instructions_list;
    int curr_instr_addr;
}parser_t;

//initializes a parser object
parser_t parser_init(lexer_t lexer);

//function that parses a program 
void parser_parse(parser_t* parser);

void parser_free(parser_t* parser);

//prints all lables declared inside the basm program
void print_all_lables();

//given a lable name returns the address it represents
int get_lable_val(char* name);
#endif