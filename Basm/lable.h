#ifndef LABLE_H
#define LABLE_H

#include <stdio.h>
#include <string.h>
#include "../dependencies/Errors/Errors.h"

typedef struct {char* lable_name; int addr; }lable_address;

int save_lable(char* name, int addr);

int lable_is_present(char* name);

int get_lable_val(char* name);

void print_all_lables();


#endif