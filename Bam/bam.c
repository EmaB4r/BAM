#include "instructions.h"
#include <stdio.h>
int main(){
    program_t program = get_program_from_path("testbasm");
    execute_program(program);
    return 0;
}