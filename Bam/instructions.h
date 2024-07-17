#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "/my_Clibs/Stack/stack.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "../dependencies/token.h"


typedef struct {char* instructions; int size;}program_t;

program_t get_program_from_path(char* path);
void execute_program(program_t program);


#endif