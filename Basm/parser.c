#include "parser.h"


typedef struct {char* lable_name; int addr}lable_address;

lable_address lable_addresses[100];
int used_address_spaces=0;

void save_lable(char* name, int addr){
    lable_addresses[used_address_spaces].lable_name=name;
    lable_addresses[used_address_spaces].addr=addr;
    used_address_spaces++;
}

int get_lable_val(char* name){
    for(int i=0; i<used_address_spaces; i++){
        if(!strcmp(lable_addresses[i].lable_name, name))
            return lable_addresses[i].addr;
    }
    printf("lable %s undeclared", name);
    panic("lable not declared");
}

void print_all_lables(){
    for(int i=0; i<used_address_spaces; i++){
        printf("%s addr %d\n", lable_addresses[i].lable_name, lable_addresses[i].addr);
    }
}



parser_t parser_init(char*source_code_path){
    parser_t parser={
        .lexer=lexer_init(source_code_path),
        .current_token=lexer_get_next_token(&parser.lexer),
        .instructions_list=list_init(),
    };
    return parser;
}


void parser_eat(parser_t* parser){
    parser->previous_token=parser->current_token;
    parser->curr_instr_addr++;
    parser->current_token=lexer_get_next_token(&parser->lexer);
}

void parser_digest(parser_t* parser, token_type expected_token){
    if(parser->current_token->type!=expected_token) {
        printf("unexpected token type;\n");
        panic("Parser Error");
    };
    parser_eat(parser);
}


void parser_parse_labledef(parser_t* parser){
    save_lable(parser->current_token->str_val, parser->curr_instr_addr);
    parser_eat(parser);
    parser->curr_instr_addr--;
}

void parser_parse_instr_with_param(parser_t* parser){
    list_ins_tail(parser->instructions_list, parser->current_token);
    parser_eat(parser);
    if(parser->current_token->type==token_num ||parser->current_token->type==token_lable){
        list_ins_tail(parser->instructions_list, parser->current_token);
        parser_eat(parser);
        parser->curr_instr_addr+=7;
    }
    else{
        printf("unexpected token after %s", parser->current_token->str_val);
        exit(1);
    }
    
}

void parser_parse_instr_without_param(parser_t*parser){
    list_ins_tail(parser->instructions_list, parser->current_token);
    parser_eat(parser);
}


void parser_parse(parser_t* parser){
    while(parser->current_token->type!=token_end){
        switch(parser->current_token->type){
            case token_lable_def: parser_parse_labledef(parser); break;
            case token_precompiler: break;
        }
        if(parser->current_token->type>=token_push && parser->current_token->type<=token_jge)
            parser_parse_instr_with_param(parser);
        if(parser->current_token->type>=token_pop && parser->current_token->type<=token_div)
            parser_parse_instr_without_param(parser);
    }
}

void parser_free(parser_t* parser){
    list_free(parser->instructions_list, token_free);
}