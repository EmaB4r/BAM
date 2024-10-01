#ifndef LABLE_H
#define LABLE_H

typedef struct {char* lable_name; int addr; }lable_address;

int save_lable(char* name, int addr);

int lables_present(char* name);

int get_lable_val(char* name);

void print_all_lables();


#endif