#include "parser.h"


// LABLE'S HANDLING WILL BE MOVED TO A SEPARATE FILE
typedef struct {char* lable_name; int addr; }lable_address;

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
//************************************

int starting_point_defined=0;


parser_t parser_init(char*source_code_path){
    //parser contains a lexer, the current and prev token and a list of parsed instructions
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

void parser_eat_noninstr(parser_t* parser){
    parser->previous_token=parser->current_token;
    parser->current_token=lexer_get_next_token(&parser->lexer);
}

void parser_digest(parser_t* parser, token_type expected_token){
    if(parser->current_token->type!=expected_token) {
        panic("\nunexpected token type after token %s on line %d\ngot %s expected %s\n",
            parser->previous_token->str_val,
            parser->lexer.current_line,
            get_token_name(parser->current_token->type),
            get_token_name(expected_token));
    }
    parser_eat(parser);
}

int parser_expect(parser_t* parser, token_type expected_token){
    return (parser->current_token->type==expected_token);
}

//saves a lable address inside the lables table
void parser_parse_labledef(parser_t* parser){
    save_lable(parser->current_token->str_val, parser->curr_instr_addr);
    parser_eat(parser);
    parser->curr_instr_addr--;
}

void parser_parse_precompiler(parser_t * parser){
    if(!strcmp(parser->current_token->str_val, "include")){
        parser_eat_noninstr(parser);
        parser_t includeparser=parser_init(parser->current_token->str_val);
        parser_parse(&includeparser);
        parser_eat_noninstr(parser);
        list_concat_tail(parser->instructions_list, includeparser.instructions_list);
    }
    if(!strcmp(parser->current_token->str_val, "var")){
        char* lablename;
        char* lableval;
        parser_eat_noninstr(parser);
        parser_expect(parser, token_lable);
        parser_eat_noninstr(parser);
        parser_expect(parser, token_num);
        save_lable(parser->previous_token->str_val, parser->current_token->num_val);
        parser_eat_noninstr(parser);
    }
    else {panic("precompiler directive %s on line %d does not exist", 
        parser->current_token->str_val, parser->lexer.current_line);}
}

//parses an instruction that expects a parameter, for example <push 10>
void parser_parse_instr_with_param(parser_t* parser){
    list_ins_tail(parser->instructions_list, parser->current_token);
    parser_eat(parser);
    //the next token has to be a number or a lable
    if(parser_expect(parser, token_num) || parser_expect(parser, token_lable)){
        list_ins_tail(parser->instructions_list, parser->current_token);
        parser_eat(parser);
        parser->curr_instr_addr+=7;
    }
    else{
        panic("\nunexpected token type after token %s on line %d\ngot %s, expected address or lable\n",
            parser->previous_token->str_val,
            parser->lexer.current_line,
            get_token_name(parser->current_token->type));
    }
    
}

//simply appends an instruction that does not expect a parameter
void parser_parse_instr_without_param(parser_t*parser){
    list_ins_tail(parser->instructions_list, parser->current_token);
    parser_eat(parser);
}


void parser_parse(parser_t* parser){
    while(parser->current_token->type!=token_end){
        //switch checks if the token is a meta-instruction
        switch(parser->current_token->type){
            case token_lable_def: parser_parse_labledef(parser); break;
            case token_precompiler: parser_parse_precompiler(parser); break;
        }
        if(instr_with_param(parser->current_token->type))
            parser_parse_instr_with_param(parser);
        if(instr_without_param(parser->current_token->type))
            parser_parse_instr_without_param(parser);
    }
}

void parser_free(parser_t* parser){
    list_free(parser->instructions_list, token_free);
    free(parser->lexer.source_code);
}