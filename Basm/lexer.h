#ifndef LEXER_H
#define LEXER_H
#include "../dependencies/token.h"
#include <stdio.h>
#include <string.h>


typedef struct {
    char* source_code;
    char current_char;
    int text_index;
}lexer_t;

lexer_t lexer_init(char* source_code_path);
void lexer_advance(lexer_t * lexer);
token_t lexer_get_next_token(lexer_t * lexer);


#endif