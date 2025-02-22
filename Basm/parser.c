#include "parser.h"
#include "lable.h"
#include "lexer.h"

// PARSER

// TODO: after the code refactor the lexer is a list of tokens
// the parser prints out parsing errors from lexer->filename
// but the preprocessor returns a single big lexer with a stream of all the tokens of the codebase
// change the error messages to print instead of lexer->filename current_token->filename

int starting_point_defined=0;

parser_t parser_init(lexer_t lexer){
    //parser contains a lexer, the current and prev token and a list of parsed instructions
    parser_t parser={
        .lexer=lexer,
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
    if(parser->current_token.type!=expected_token) {
        panic("\nunexpected token type after token %s on line %d\ngot %s expected %s\n",
            parser->previous_token.str_val,
            parser->current_token.line,
            get_token_name(parser->current_token.type),
            get_token_name(expected_token));
    }
    parser_eat(parser);
}

int parser_expect(parser_t* parser, token_type expected_token){
    return (parser->current_token.type==expected_token);
}


void precompiler_include(parser_t * parser){
    parser_eat_noninstr(parser);
    parser_t includeparser=parser_init(lexer_init(parser->current_token.str_val));
    includeparser.curr_instr_addr=parser->curr_instr_addr;
    parser_parse(&includeparser);
    parser_eat_noninstr(parser);
    list_append(parser->instructions_list, includeparser.instructions_list);
    parser->curr_instr_addr=includeparser.curr_instr_addr;
    parser_free(&includeparser);
}


void precompiler_def(parser_t * parser){
    parser_eat_noninstr(parser);
    parser_expect(parser, token_lable);
    parser_eat_noninstr(parser);
    parser_expect(parser, token_num);
    if(!save_lable(parser->previous_token.str_val, parser->current_token.num_val)){
        panic("redefinition of global '%s' at line %d of %s\n",
            parser->previous_token.str_val, 
            parser->current_token.line,
            parser->lexer.filename);
    }
    parser_eat_noninstr(parser);
}

void precompiler_asciiz(parser_t *parser){
    parser_eat_noninstr(parser);
    LIST_INS_TAIL(parser->instructions_list, parser->current_token);
    parser->curr_instr_addr+=strlen(parser->current_token.str_val);
    parser_eat_noninstr(parser);
}

void precompiler_byte(parser_t* parser){
    LIST_INS_TAIL(parser->instructions_list, parser->current_token);
    parser_eat_noninstr(parser);
    LIST_INS_TAIL(parser->instructions_list, parser->current_token);
    parser_eat_noninstr(parser);
    if(parser->previous_token.type==token_num) parser->curr_instr_addr += parser->previous_token.num_val;
    else parser->curr_instr_addr+=get_lable_val(parser->previous_token.str_val)*sizeof(uint64_t);
}

//saves a lable address inside the lables table
void parser_parse_labledef(parser_t* parser){
    if(!save_lable(parser->current_token.str_val, parser->curr_instr_addr)){
        panic("redefinition of lable '%s' at line %d of %s\n", 
            parser->current_token.str_val, 
            parser->current_token.line,
            parser->lexer.filename);
    }
    parser_eat(parser);
    parser->curr_instr_addr--;
}

void print_tok(token_t *tok){
    printf("%s %s %lu\n", get_token_name(tok->type), tok->str_val, tok->num_val);
}

//parses an instruction that expects a parameter, for example <push 10>
void parser_parse_instr_with_param(parser_t* parser){
    LIST_INS_TAIL(parser->instructions_list, parser->current_token);
    parser_eat(parser);
    //the next token has to be a number or a lable
    if(parser_expect(parser, token_num) || parser_expect(parser, token_lable)){
        LIST_INS_TAIL(parser->instructions_list, parser->current_token);
        parser_eat(parser);
        parser->curr_instr_addr+=7;
    }
    else{
        panic("\nunexpected token type after token %s on line %d\ngot %s, expected address or lable\n",
            parser->previous_token.str_val,
            parser->current_token.line,
            get_token_name(parser->current_token.type));
    }
    
}

//simply appends an instruction that does not expect a parameter
void parser_parse_instr_without_param(parser_t*parser){
    LIST_INS_TAIL(parser->instructions_list, parser->current_token);
    parser_eat(parser);
}
void parser_parse_lable(parser_t*parser){
    LIST_INS_TAIL(parser->instructions_list, parser->current_token);
    parser_eat_noninstr(parser);
}


void parser_parse(parser_t* parser){
    while(parser->current_token.type!=token_end){
        //switch checks if the token is a meta-instruction
        switch(parser->current_token.type){
            case token_lable: parser_parse_lable(parser); break;
            case token_lable_def: parser_parse_labledef(parser); break;
            case token_precompiler_def: precompiler_def(parser); break;
            case token_precompiler_include: precompiler_include(parser); break;
            case token_precompiler_asciiz: precompiler_asciiz(parser); break;
            case token_precompiler_byte: precompiler_byte(parser); break;
            //case token_precompiler_macro_def : precompiler_macro(parser); break;
            //case token_precompiler_end_macro_def: return; break;
        }
        if(instr_with_param(parser->current_token.type))
            parser_parse_instr_with_param(parser);
        if(instr_without_param(parser->current_token.type))
            parser_parse_instr_without_param(parser);
    }
}

void parser_free(parser_t* parser){
    list_free(parser->instructions_list);
    free(parser->lexer.source_code);
}
