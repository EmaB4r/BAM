#include "instructions.h"
#include <stdint.h>
program_t get_program_from_path(char* path){
    FILE *f=fopen(path, "r");
    char * buffer = 0;
    int length;
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
    program_t prog={.instructions=buffer, .size=length};
    return prog;
}


void execute_program(program_t program){
    unsigned char curr_instr;
    uint64_t operand;
    uint64_t a;
    uint64_t b;
    uint64_t result;
    stack_t help_stack;
    stack_t subroutine_stack=sstack_init(200);
    int PC = 0;
    while(PC<program.size){
        curr_instr=program.instructions[PC];
        PC++;
        if(instr_with_param(curr_instr)){
            BYTECOPY(&operand, program.instructions+PC, sizeof(uint64_t));
            PC+=8;
            switch(curr_instr){
                case token_push: STACK_PUSH(operand); break;
                case token_roll: 
                    help_stack=sstack_init(operand*sizeof(uint64_t));
                    a = STACK_POP(uint64_t);
                    b=1;
                    while (b<operand){
                        result=STACK_POP(uint64_t);
                        SSTACK_PUSH(help_stack, result);
                        b++;
                    }
                    STACK_PUSH(a);
                    while(help_stack->used_space){
                        a=SSTACK_POP(help_stack, uint64_t);
                        STACK_PUSH(a);
                    }
                    sstack_free(help_stack);
                    break;
                    
                case token_jmp: PC=operand; break;
                case token_jeq: 
                    a=STACK_POP(uint64_t);
                    b=STACK_POP(uint64_t);
                    PC= (a==b) ? operand : PC;
                    break;
                case token_jne:
                    a=STACK_POP(uint64_t);
                    b=STACK_POP(uint64_t);
                    PC= (a!=b) ? operand : PC;
                    break;
                case token_jlt: 
                    a=STACK_POP(uint64_t);
                    b=STACK_POP(uint64_t);
                    PC= (a<b) ? operand : PC;
                    break;
                case token_jle:
                    a=STACK_POP(uint64_t);
                    b=STACK_POP(uint64_t);
                    PC= (a<=b) ? operand : PC;
                    break;
                case token_jgt: 
                    a=STACK_POP(uint64_t);
                    b=STACK_POP(uint64_t);
                    PC= (a>b) ? operand : PC;
                    break;
                case token_jge:
                    a=STACK_POP(uint64_t);
                    b=STACK_POP(uint64_t);
                    PC= (a>=b) ? operand : PC;
                    break;
                case token_jsr:
                    SSTACK_PUSH(subroutine_stack, PC);
                    PC=operand;
                    break;
            }
        };
        if(instr_without_param(curr_instr)){
            switch (curr_instr) {
                case token_stop: exit(0); break;
                case token_pop: STACK_POP(uint64_t); break;
                case token_dup: 
                    a=STACK_POP(uint64_t); 
                    STACK_PUSH(a); 
                    STACK_PUSH(a); 
                    break;
                case token_swap: 
                    a=STACK_POP(uint64_t); 
                    b=STACK_POP(uint64_t); 
                    STACK_PUSH(a);
                    STACK_PUSH(b); 
                    break;
                case token_add: 
                    a=STACK_POP(uint64_t); 
                    b=STACK_POP(uint64_t); 
                    a+=b; 
                    STACK_PUSH(a);
                    break;
                case token_sub: 
                    a=STACK_POP(uint64_t); 
                    b=STACK_POP(uint64_t); 
                    a-=b; 
                    STACK_PUSH(a);
                    break;
                case token_mul: 
                    a=STACK_POP(uint64_t); 
                    b=STACK_POP(uint64_t); 
                    a*=b; 
                    STACK_PUSH(a);
                    break;
                case token_div: 
                    a=STACK_POP(uint64_t); 
                    b=STACK_POP(uint64_t); 
                    result= a%b; 
                    STACK_PUSH(result);
                    result= a/b; 
                    STACK_PUSH(result);
                    break;
                case token_print: printf("%lu\n", STACK_POP(uint64_t));
                    break;
                case token_rts: PC=SSTACK_POP(subroutine_stack, int);
            }
        };
        
        
    }
    
    
}