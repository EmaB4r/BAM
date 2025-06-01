#include "lexer.h"
#include "../dependencies/Errors/Errors.h"

int char_is_num(char c){return (c>='0' && c<='9');}

int char_is_blank(char c){return (c==' '||c=='\n');}


int lexer_strtouint64(char*str){
    int i=0;
    while(*str!='\0'){
        i=i*10+(*str-'0');
        str++;
    }
    return i;
}

char* lexer_collect_str(lexer_t* lexer){
    char*str=calloc(50, sizeof(char));
    int i=0;
    while(!char_is_blank(lexer->current_char) && lexer->current_char!='\0' ){
        str[i]=lexer->current_char;
        i++;
        lexer_advance(lexer);
    }
    return str;
}

char* lexer_collect_strstr(lexer_t * lexer){
    char*str=calloc(50, sizeof(char));
    int i=0;
    while(lexer->current_char!='\"' ){
        str[i]=lexer->current_char;
        i++;
        lexer_advance(lexer);
    }
    lexer_advance(lexer);
    return str;
}

char lexer_collect_char(lexer_t * lexer){
    char c;
    if(lexer->current_char=='\\'){
        lexer_advance(lexer);
        switch(lexer->current_char){
            case 'n': c='\n'; break;
        }
        lexer_advance(lexer);
        lexer_advance(lexer);
        return c;
    }
    c=lexer->current_char;
    lexer_advance(lexer);
    lexer_advance(lexer);
    return c;
}



char * get_file_text_from_path(char* path){
    FILE *f=fopen(path, "r");
    if(f==NULL) panic("unable to open file %s", path);
    char * buffer = 0;
    long length;
    if (f){
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = calloc (1, length+1);
        if (buffer){
            fread (buffer, 1, length, f);
        }
        fclose (f);
    }
    return buffer;
}

token_t lexer_tokenize(lexer_t * lexer);
lexer_t lexer_init(char* source_code_path){
    lexer_t lexer = {.source_code = get_file_text_from_path(source_code_path),
                     .text_index=0,
                     .current_char=lexer.source_code[lexer.text_index],
                     .current_line=0,
                     .filename=source_code_path,
                     .lexemes=list_init(),
                     .current_node=NULL,
    };
    token_t token;
    do{
        token=lexer_tokenize(&lexer);
        LIST_INS_TAIL(lexer.lexemes, token);
    }while(token.type!=token_end);
    return lexer;
}

void lexer_advance(lexer_t * lexer){
    lexer->text_index++;
    lexer->current_char=lexer->source_code[lexer->text_index];
}

void lexer_skip_commen_line(lexer_t * lexer){
    while(lexer->current_char!='\n' && lexer->current_char!='\0'){
        lexer_advance(lexer);
    }
    lexer->current_line++;
}

void lexer_skip_comment_block(lexer_t * lexer){
    while(lexer->current_char!='\0'){
        if(lexer->current_char=='*'){
            lexer_advance(lexer);
            if(lexer->current_char=='/' && lexer->current_char!='\0'){
                lexer_advance(lexer);
                return;
            }
        }
        else lexer_advance(lexer);
    }
}

void lexer_skip_blanks(lexer_t * lexer){
    while(char_is_blank(lexer->current_char) && lexer->current_char!='\0'){
        if (lexer->current_char=='\n') lexer->current_line++;
        lexer_advance(lexer);
    }
}

void lexer_reset(lexer_t * lexer){
    lexer->current_node=NULL;
}


token_t lexer_tokenize(lexer_t * lexer){
    lexer_skip_blanks(lexer);
    char*instr_name;
    
    //block used to determine if there is a comment and if it's either a `//comment` or `/* comment */`
    while (lexer->current_char=='/'){
        lexer_advance(lexer);
        if(lexer->current_char=='/') lexer_skip_commen_line(lexer);
        else if (lexer->current_char =='*') lexer_skip_comment_block(lexer);
        else {panic("comment at line %d wrongly written\n", lexer->current_line);} 
        lexer_skip_blanks(lexer);
    }
    
    //check if the current char is a number
    if(char_is_num(lexer->current_char)){
        token_t tok = token_init(token_num, lexer_collect_str(lexer), 0, lexer->current_line);
        tok.num_val=lexer_strtouint64(tok.str_val);
        return tok;
    }
    
    //switch checks for all possible instructions
    switch(lexer->current_char){
        //'\0' used to stop the lexing process
        case '\0': return token_init(token_end, NULL, 0, lexer->current_line); break;
        
        //'@' used for lable definitions
        case '@': 
            lexer_advance(lexer); 
            return token_init(token_lable_def, lexer_collect_str(lexer), 0, lexer->current_line); 
        break;
        
        //'.' used for precompiler instructions
        case '.':
            lexer_advance(lexer);
            instr_name = lexer_collect_str(lexer);
            if(!strcmp(instr_name, "def")) return token_init(token_precompiler_def, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "ifdef")) return token_init(token_precompiler_ifdef, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "ifndef")) return token_init(token_precompiler_ifndef, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "endif")) return token_init(token_precompiler_endif, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "macro")) return token_init(token_precompiler_macro_def, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "endmacro")) return token_init(token_precompiler_end_macro_def, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "include")) return token_init(token_precompiler_include, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "asciiz")) return token_init(token_precompiler_asciiz, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "byte")) return token_init(token_precompiler_byte, instr_name, 0, lexer->current_line);
        
            panic("%s:%d unknown precompiler directive .%s\n", 
                lexer->filename, lexer->current_line+1,
                instr_name
            );
            break;
        
        case '\'':
            lexer_advance(lexer);
            return token_init(token_num, NULL, lexer_collect_char(lexer), lexer->current_line);
        break;
        
        case '\"':
            lexer_advance(lexer);
            return token_init(token_str, lexer_collect_strstr(lexer), 0, lexer->current_line);
        break;
        
        //checks for all possible instructions and if not present returns a lable
        default: 
            //in future will probably become a binary search tree if the number of instruction continues to increase
            instr_name = lexer_collect_str(lexer);
            if(!strcmp(instr_name, "push")) return token_init(token_push, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "pop")) return token_init(token_pop, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "add")) return token_init(token_add, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "sub")) return token_init(token_sub, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "mul")) return token_init(token_mul, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "div")) return token_init(token_div, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "addi")) return token_init(token_addi, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "subi")) return token_init(token_subi, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "muli")) return token_init(token_muli, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "divi")) return token_init(token_divi, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "dup")) return token_init(token_dup, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "swap")) return token_init(token_swap, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "roll")) return token_init(token_roll, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "syscall")) return token_init(token_syscall, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "print_stack")) return token_init(token_print_stack, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jmp")) return token_init(token_jmp, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jeq")) return token_init(token_jeq, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jne")) return token_init(token_jne, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jlt")) return token_init(token_jlt, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jgt")) return token_init(token_jgt, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jle")) return token_init(token_jle, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jge")) return token_init(token_jge, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "jsr")) return token_init(token_jsr, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "rts")) return token_init(token_rts, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "stop")) return token_init(token_stop, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "and")) return token_init(token_and, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "or")) return token_init(token_or, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "not")) return token_init(token_not, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "andi")) return token_init(token_andi, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "ori")) return token_init(token_ori, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "noti")) return token_init(token_noti, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "shl")) return token_init(token_shl, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "shr")) return token_init(token_shr, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "shli")) return token_init(token_shli, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "shri")) return token_init(token_shri, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "lb")) return token_init(token_lb, instr_name, 0, lexer->current_line);
            if(!strcmp(instr_name, "sb")) return token_init(token_sb, instr_name, 0, lexer->current_line);
            
            else return token_init(token_lable, instr_name, 0, lexer->current_line);
        break;
    }
}

token_t lexer_get_next_token(lexer_t * lexer){
    if(lexer->current_node==NULL) lexer->current_node=lexer->lexemes->head;
    else if (((token_t *)(lexer->current_node->item))->type!=token_end) 
        lexer->current_node = lexer->current_node->next;
    return *((token_t *)(lexer->current_node->item));
};