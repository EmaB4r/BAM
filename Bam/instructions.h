#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "../dependencies/Stack/stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dependencies/token.h"


typedef struct {char* instructions; int size;}program_t;


//returns a binary file wrapped inside a program struct, containing the size and the program intself
program_t get_program_from_path(char* path);

//runs a program
void execute_program(program_t program);


#endif