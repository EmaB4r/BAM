#include "parser.h"
#include <stdio.h>




void write_instructions(list_t list, FILE *fout){
    node_t head = list_get_head(list);
    int val;
    
    while(head!=NULL){
        token_t curr_tok = (token_t)head->Item;
        
        if(curr_tok->type>=token_push && curr_tok->type<=token_jge){
            fwrite(&curr_tok->type, 1, 1, fout);
            head=head->next;
            token_t curr_tok = (token_t)head->Item;
            if(curr_tok->type==token_lable){
                curr_tok->num_val=get_lable_val(curr_tok->str_val);
            }
            fwrite(&curr_tok->num_val, sizeof(uint64_t), 1, fout);
        }
           
        if(curr_tok->type>=token_pop && curr_tok->type<=token_div)
            fwrite(&curr_tok->type, 1, 1, fout);
            
        
        head=head->next;
    }
    fclose(fout);
}



int main(int argc, char*argv[]){
    if(argc<2){
        printf("use basm <source_code.basm>\n");
        exit(0);
    }
    
    parser_t parser=parser_init(argv[1]);
    parser_parse(&parser);
    print_all_lables();
    argv[1][strlen(argv[1])-5]='\0';
    write_instructions(parser.instructions_list, fopen(argv[1], "wb"));
    return 0;
}