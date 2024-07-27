#ifndef LEXER_H
#define LEXER_H
#include "../dependencies/token.h"
#include <stdio.h>
#include <string.h>


typedef struct {
    char* source_code;
    char* filename;
    int text_index;
    int current_line;
    char current_char;
}lexer_t;

//initializes a lexer object
lexer_t lexer_init(char* source_code_path);

//advances the lexer.text_index and updates lexer.current_char
void lexer_advance(lexer_t * lexer);

//the current token
token_t lexer_get_next_token(lexer_t * lexer);


#endif