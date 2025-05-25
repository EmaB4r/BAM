#include "preprocessor.h"
#include "lable.h"
#include "lexer.h"


macro_t macros[200];
int n_macros=0;
void save_macro();

int macro_is_present(char* name){
    for(int i=0; i<n_macros; i++){
        if(!strcmp(macros[i].name, name))
            return i;
    }
    return -1;
}

void precompiler_macro(lexer_t * lexer){
    token_t current_token = lexer_get_next_token(lexer);
    if(macro_is_present(current_token.str_val) != -1){
        panic("Redefinition of macro %s\n", current_token.str_val);
    }
    macro_t * current_macro=&(macros[n_macros++]);
    //goes to the next token after .macro, expects a lable representing the macro name
    if(current_token.type!=token_lable){
        printf("%s:%d: Expected a macro name, got instead %s\n", 
            lexer->filename, current_token.line, current_token.str_val);
        exit(1);
    }
    
    //copy the string reference, since no frees happen
    current_macro->name = current_token.str_val;
    
    
    current_token = lexer_get_next_token(lexer);
    current_macro->body = list_init();
    
    while(current_token.type!=token_precompiler_end_macro_def){
        LIST_INS_TAIL(current_macro->body, current_token);
        current_token = lexer_get_next_token(lexer);
    }
    current_token=token_init(token_end, NULL, 0, 0); //append a token_end for parsing purpose
    LIST_INS_TAIL(current_macro->body, current_token);
    
    lexer_t macro_lexer=*lexer; //dummy copy of the lexer, only useful stuff is the filename
    macro_lexer.current_node=NULL;
    macro_lexer.lexemes=current_macro->body;
    parser_t macro_parser = parser_init(macro_lexer);
    parser_parse(&macro_parser);
    
    //cut off the token_end in the tail
    // TODO: improve list.h for implementing proper truncation,
    // like list_delete_head and tail, also maybe some more appending/joining features
    node_t *tail = current_macro->body->tail;
    tail=tail->prev;
    tail->next=NULL;
    current_macro->body->tail=tail;
}

void precompiler_def(lexer_t * lexer){
    token_t def_name = lexer_get_next_token(lexer);
    token_t def_val = lexer_get_next_token(lexer);
    if(!save_lable(def_name.str_val, def_val.num_val)){
        panic("redefinition of global '%s' at line %d of %s\n",
            def_name.str_val, 
            def_name.line,
            lexer->filename);
    }
}

void precompiler_ifndef(lexer_t * processed, lexer_t * pending){
    token_t current_token = lexer_get_next_token(pending);
    if(lable_is_present(current_token.str_val)){
        while(lexer_get_next_token(pending).type != token_precompiler_endif);
    }
}

void precompiler_inlcude(lexer_t * processed, lexer_t * pending){
    lexer_t include_lexer = process(lexer_get_next_token(pending).str_val, 0);
    list_append(processed->lexemes, include_lexer.lexemes);
}

void expand_macro(int macro_index, lexer_t * pending_lexer){
    macro_t * current_macro=&(macros[macro_index]);
    list_append_after_node(pending_lexer->current_node, current_macro->body);
}

lexer_t process(char*filename, int add_terminator){
    lexer_t pending_lexer=lexer_init(filename);
    lexer_t processed_lexer=pending_lexer;
    processed_lexer.lexemes=list_init();
    token_t current_token=lexer_get_next_token(&pending_lexer);
    while(current_token.type!=token_end){
        switch(current_token.type){
            case token_precompiler_macro_def : precompiler_macro(&pending_lexer); break;
            case token_precompiler_ifndef: precompiler_ifndef(&processed_lexer, &pending_lexer);
            case token_precompiler_endif: break;
            case token_precompiler_include: precompiler_inlcude(&processed_lexer, &pending_lexer); break;
            case token_precompiler_def: precompiler_def(&pending_lexer); break;
            case token_lable: {
                int macro_index = macro_is_present(current_token.str_val);
                if(macro_index!=-1){expand_macro(macro_index, &pending_lexer); break; }
            }
            default: LIST_INS_TAIL(processed_lexer.lexemes, current_token); break;
        }
        current_token=lexer_get_next_token(&pending_lexer);
    }
    //list_free(lexer.lexemes);
    if(add_terminator){
        token_t token=token_init(token_end, NULL, 0, 0);
        LIST_INS_TAIL(processed_lexer.lexemes, token);
    }
    lexer_reset(&processed_lexer);
    return processed_lexer;
}