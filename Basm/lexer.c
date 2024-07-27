#include "lexer.h"
#include "../dependencies/Errors/Errors.h"

int char_is_num(char c){return (c>='0' && c<='9');}

int char_is_blank(char c){return (c==' '||c=='\n');}


int lexer_collect_num(lexer_t* lexer){
    int i=0;
    while(!char_is_blank(lexer->current_char) && lexer->current_char!='\0'){
        i=i*10+lexer->current_char-'0';
        lexer_advance(lexer);
    }
    return i;
}

char* lexer_collect_str(lexer_t* lexer){
    char*str=calloc(50, sizeof(char));
    int i=0;
    while(!char_is_blank(lexer->current_char) && lexer->current_char!='\0'){
        str[i]=lexer->current_char;
        i++;
        lexer_advance(lexer);
    }
    return str;
}

char * get_file_text_from_path(char* path){
    FILE *f=fopen(path, "r");
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


lexer_t lexer_init(char* source_code_path){
    lexer_t lexer = {.source_code = get_file_text_from_path(source_code_path),
                     .text_index=0};
    lexer.current_char=lexer.source_code[lexer.text_index];
    lexer.current_line=0;
    lexer.filename=source_code_path;
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



token_t lexer_get_next_token(lexer_t * lexer){
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
        return token_init(token_num, NULL, lexer_collect_num(lexer));
    }
    
    //switch checks for all possible instructions
    switch(lexer->current_char){
        //'\0' used to stop the lexing process
        case '\0': return token_init(token_end, NULL, 0); break;
        
        //'@' used for lable definitions
        case '@': 
            lexer_advance(lexer); 
            return token_init(token_lable_def, lexer_collect_str(lexer), 0); 
        break;
        
        //'.' used for precompiler instructions
        case '.':
            lexer_advance(lexer); 
            return token_init(token_precompiler, lexer_collect_str(lexer), 0); 
        break;
        
        //checks for all possible instructions and if not present returns a lable
        default: 
            //in future will probably become a binary search tree if the number of instruction continues to increase
            instr_name = lexer_collect_str(lexer);
            if(!strcmp(instr_name, "push")) return token_init(token_push, instr_name, 0);
            if(!strcmp(instr_name, "pop")) return token_init(token_pop, instr_name, 0);
            if(!strcmp(instr_name, "add")) return token_init(token_add, instr_name, 0);
            if(!strcmp(instr_name, "sub")) return token_init(token_sub, instr_name, 0);
            if(!strcmp(instr_name, "mul")) return token_init(token_mul, instr_name, 0);
            if(!strcmp(instr_name, "div")) return token_init(token_div, instr_name, 0);
            if(!strcmp(instr_name, "dup")) return token_init(token_dup, instr_name, 0);
            if(!strcmp(instr_name, "swap")) return token_init(token_swap, instr_name, 0);
            if(!strcmp(instr_name, "roll")) return token_init(token_roll, instr_name, 0);
            if(!strcmp(instr_name, "print")) return token_init(token_print, instr_name, 0);
            if(!strcmp(instr_name, "print_stack")) return token_init(token_print_stack, instr_name, 0);
            if(!strcmp(instr_name, "jmp")) return token_init(token_jmp, instr_name, 0);
            if(!strcmp(instr_name, "jeq")) return token_init(token_jeq, instr_name, 0);
            if(!strcmp(instr_name, "jne")) return token_init(token_jne, instr_name, 0);
            if(!strcmp(instr_name, "jlt")) return token_init(token_jlt, instr_name, 0);
            if(!strcmp(instr_name, "jgt")) return token_init(token_jgt, instr_name, 0);
            if(!strcmp(instr_name, "jle")) return token_init(token_jle, instr_name, 0);
            if(!strcmp(instr_name, "jge")) return token_init(token_jge, instr_name, 0);
            if(!strcmp(instr_name, "jsr")) return token_init(token_jsr, instr_name, 0);
            if(!strcmp(instr_name, "rts")) return token_init(token_rts, instr_name, 0);
            if(!strcmp(instr_name, "stop")) return token_init(token_stop, instr_name, 0);
            else return token_init(token_lable, instr_name, 0);
        break;
    }
}

