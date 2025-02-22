#include "preprocessor.h"
#include "lexer.h"
#include "parser.h"


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
    macro_t * current_macro=&(macros[n_macros++]);
    token_t current_token = lexer_get_next_token(lexer); 
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

// TODO: pendind_lexer->current_token is a lable representing a macro name
// to that token should be appended the macro body
// like current_macro->body->tail->next=pending_lexer->current_node->next;
// like pending_lexer->current_node->next=current_macro->body->head
// basically doing
// macro_name -> something
// macro_name -> macro_instr1 -> macro_instr2 -> ... -> something
// 
// DANGER!!
// This works only when you have macros that do not call other macros
// because if you do macro1 {macro2 macro2} this would expand macro2 into the actual macro1 body, doing a mess
// should instead of changing pointers create a copy of the body and actually insert it into the pending lexer
void expand_macro(int macro_index, lexer_t * pending_lexer){
    node_t *node_before_macro=pending_lexer->current_node;
    node_t *node_after_macro=node_before_macro->next;
    macro_t * current_macro=&(macros[macro_index]);
    node_before_macro->next=current_macro->body->head;
    current_macro->body->tail->next=node_after_macro;
}

lexer_t process(char*filename){
    lexer_t pending_lexer=lexer_init(filename);
    lexer_t processed_lexer=pending_lexer;
    processed_lexer.lexemes=list_init();
    token_t current_token=lexer_get_next_token(&pending_lexer);
    while(current_token.type!=token_end){
        switch(current_token.type){
            case token_precompiler_macro_def : precompiler_macro(&pending_lexer); break;
            case token_lable: {
                int macro_index = macro_is_present(current_token.str_val);
                if(macro_index!=-1){expand_macro(macro_index, &pending_lexer); break; }
            }
            default: LIST_INS_TAIL(processed_lexer.lexemes, current_token); break;
        }
        current_token=lexer_get_next_token(&pending_lexer);
    }
    //list_free(lexer.lexemes);
    token_t token=token_init(token_end, NULL, 0, 0);
    LIST_INS_TAIL(processed_lexer.lexemes, token);
    lexer_reset(&processed_lexer);
    return processed_lexer;
}