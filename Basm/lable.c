#include "lable.h"
#include <stdio.h>
#include <string.h>
#include "../dependencies/Errors/Errors.h"

lable_address lable_addresses[200];
int used_address_spaces=0;

int lable_is_present(char* name){
    for(int i=0; i<used_address_spaces; i++){
        if(!strcmp(lable_addresses[i].lable_name, name))
            return 1;
    }
    return 0;
}


int save_lable(char* name, int addr){
    if(lable_is_present(name)) return 0;
    lable_addresses[used_address_spaces].lable_name=name;
    lable_addresses[used_address_spaces].addr=addr;
    used_address_spaces++;
    return 1;
}

int get_lable_val(char* name){
    for(int i=0; i<used_address_spaces; i++){
        if(!strcmp(lable_addresses[i].lable_name, name))
            return lable_addresses[i].addr;
    }
    printf("lable %s undeclared\n", name);
    panic("lable not declared");
}

void print_all_lables(){
    for(int i=0; i<used_address_spaces; i++){
        printf("%s addr %d\n", lable_addresses[i].lable_name, lable_addresses[i].addr);
    }
}