#include "instructions.h"
#include <stdio.h>
int main(int argc, char*argv[]){
    if(argc<2){
        printf("useage:\n bam <program>\n");
        exit(0);
    }
    program_t program = get_program_from_path(argv[1]);
    execute_program(program);
    return 0;
}