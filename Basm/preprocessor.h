#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include "../dependencies/List/list.h"
#include "lexer.h"
#include "parser.h"
#include "lable.h"

typedef struct macro_s{
    char* name;
    list_t * body;
    list_t * parameters;
}macro_t;

lexer_t process(char*filename, int add_terminator);


#endif