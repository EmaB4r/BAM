#include "lexer.h"
#include "parser.h"
#include "preprocessor.h"
#include <stdint.h>
#include <stdio.h>




void write_instructions(list_t * list, FILE *fout){
    node_t * head = list->head;
    int val;
    
    while(head!=NULL){
        token_t *curr_tok = (token_t*)head->item;
        
        if(curr_tok->type>=token_push && curr_tok->type<=token_jge){
            fwrite(&curr_tok->type, 1, 1, fout);
            head=head->next;
            curr_tok = (token_t*)head->item;
            if(curr_tok->type==token_lable){
                curr_tok->num_val=get_lable_val(curr_tok->str_val);
            }
            fwrite(&curr_tok->num_val, sizeof(uint64_t), 1, fout);
        }  
        else if(curr_tok->type>=token_pop && curr_tok->type<=token_div)
            fwrite(&curr_tok->type, 1, 1, fout);
        else if(curr_tok->type==token_str){
            fwrite(curr_tok->str_val, 1, strlen(curr_tok->str_val), fout);
        }
        else if(curr_tok->type==token_precompiler_byte){
            head=head->next;
            curr_tok=head->item;
            uint64_t z = 0x0;
            if(curr_tok->type==token_lable){
                curr_tok->num_val=get_lable_val(curr_tok->str_val);
            }
            for(int i=0; i<curr_tok->num_val; i++) fwrite(&z, sizeof(uint64_t), 1, fout);
        }
        else if(curr_tok->type == token_lable){
            curr_tok->num_val=get_lable_val(curr_tok->str_val);
            fwrite(&curr_tok->num_val, sizeof(uint64_t), 1, fout);
        }
        
        head=head->next;
    }
    fclose(fout);
}

void write_lexer_file(lexer_t * lexer){
    node_t *current_node = lexer->lexemes->head;
    FILE * fout = fopen("output.txt", "w");
    do{
        fputs(get_token_name(((token_t *)current_node->item)->type), fout);
        fputc(' ', fout);
        fputs(
            (((token_t *)current_node->item)->str_val == NULL)? "NULL" : ((token_t *)current_node->item)->str_val,
            fout);
        //fputs(((token_t *)current_node->item)->str_val, fout);
        fputc('\n', fout);
        current_node = current_node->next;
    }while(((token_t *)current_node->item)->type != token_end);
    fclose(fout);
}


int main(int argc, char*argv[]){
    if(argc<2){
        printf("usage:\nbasm <source_code.asm> [optional out-name]\n");
        exit(0);
    }
    parser_t parser=parser_init(process(argv[1], 1));
    token_t jmp_to_main = token_init(token_jmp, "jmp", 0, -1);
    LIST_INS_TAIL(parser.instructions_list, jmp_to_main);
    jmp_to_main = token_init(token_lable, "main", 0, -1);
    LIST_INS_TAIL(parser.instructions_list, jmp_to_main);
    //write_lexer_file(&parser.lexer);
    parser.curr_instr_addr+=9;
    parser_parse(&parser);
    char* outfile_name=(argc<3) ? "b.out" : argv[2]; 
    write_instructions(parser.instructions_list, fopen(outfile_name, "wb"));
    parser_free(&parser);
    return 0;
}