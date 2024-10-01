#include "instructions.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//#define DEBUG 
#ifdef DEBUG
    #define db_type(t) print_type(t)
    #define db_opr(o) printf("%d\n", o)
    #define db_nopr() putc('\n', stdout)
    #define db_print(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
    #define db_type(t) 
    #define db_opr(o) 
    #define db_nopr() 
    #define db_print(fmt, ...)
#endif



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

token_type curr_instr;
uint64_t lbval;
uint64_t operand;
uint64_t a;
uint64_t b;
uint64_t result;
stack_t help_stack;
stack_t subroutine_stack;
stack_t program_stack;
int PC = 0;

void syscall(){
    char ch;
    uint64_t num;
    uint64_t syscode = *(uint64_t*)stack_pop(&program_stack);
    switch(syscode){
        case 0:
            ch=*(uint64_t*)stack_pop(&program_stack);
            putc(ch, stdout);
        break;
        case 2:
            num=*(uint64_t*)stack_pop(&program_stack);
            printf("%lu", num);
        break;
        default: printf("unknown syscall argument, got %lu\nlook into documentation for syscall codes\n", syscode); exit(1);;
        break;
        
    }
}


void execute_program(program_t program){
    subroutine_stack=stack_init(200, sizeof(uint64_t));
    program_stack=stack_init(200, sizeof(uint64_t));
    while(PC<program.size){
        db_print("PC: %d ", PC);
        curr_instr=program.instructions[PC];
        PC++;
        db_type(curr_instr);
        if(instr_with_param(curr_instr)){
            memcpy(&operand, program.instructions+PC, sizeof(uint64_t));
            db_print(" %d\n", operand);
            PC+=8;
            switch(curr_instr){
                case token_addi: 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    a+=operand;
                    stack_push(&program_stack, &a);
                break;
                case token_subi: 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    a+=operand;
                    stack_push(&program_stack, &a);
                break;
                case token_muli: assert(0 && "muli not implemented yet"); break;
                case token_divi: assert(0 && "divi not implemented yet"); break;
                case token_push: stack_push(&program_stack, &operand); break;
                case token_roll: 
                    help_stack=stack_init(operand, sizeof(uint64_t));
                    a = *(uint64_t *)stack_pop(&program_stack);
                    b=1;
                    while (b<operand){
                        result=*(uint64_t *)stack_pop(&program_stack);
                        stack_push(&help_stack, &result);
                        b++;
                    }
                    stack_push(&program_stack, &a);
                    while(help_stack.used_space){
                        a=*(uint64_t *)stack_pop(&help_stack);
                        stack_push(&program_stack, &a);
                    }
                    stack_free(&help_stack);
                    break;
                case token_dup: 
                    a=operand-1;
                    do{
                        operand--;
                        stack_push(&program_stack, stack_peek(&program_stack, a));
                    }while(operand>0);
                    break;
                case token_jmp: PC=operand; break;
                case token_jeq: 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    b=*(uint64_t*)stack_pop(&program_stack);
                    PC= (a==b) ? operand : PC;
                    break;
                case token_jne:
                    a=*(uint64_t*)stack_pop(&program_stack);
                    b=*(uint64_t*)stack_pop(&program_stack);
                    PC= (a!=b) ? operand : PC;
                    break;
                case token_jlt: 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    b=*(uint64_t*)stack_pop(&program_stack);
                    PC= (a<b) ? operand : PC;
                    break;
                case token_jle:
                    a=*(uint64_t*)stack_pop(&program_stack);
                    b=*(uint64_t*)stack_pop(&program_stack);
                    PC= (a<=b) ? operand : PC;
                    break;
                case token_jgt: 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    b=*(uint64_t*)stack_pop(&program_stack);
                    PC= (a>b) ? operand : PC;
                    break;
                case token_jge:
                    a=*(uint64_t*)stack_pop(&program_stack);
                    b=*(uint64_t*)stack_pop(&program_stack);
                    PC= (a>=b) ? operand : PC;
                    break;
                case token_jsr:
                    stack_push(&subroutine_stack, &PC);
                    PC=operand;
                    break;
                case token_andi:
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a & operand;
                    stack_push(&program_stack, &result);
                break;
                case token_ori:  
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a | operand;
                    stack_push(&program_stack, &result);    
                break;
                case token_noti:
                    result = operand^0xFFFFFFFF;
                    stack_push(&program_stack, &result);
                break;
                case token_shli:  
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a << operand;
                    stack_push(&program_stack, &result);   
                break;
                case token_shri: 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a >> operand;
                    stack_push(&program_stack, &result);   
                break;
            }
        };
        if(instr_without_param(curr_instr)){
            db_nopr();
            
            switch (curr_instr) {
                case token_stop: exit(0); break;
                case token_pop: *(uint64_t*)stack_pop(&program_stack); break;
                case token_swap: 
                    a=*(uint64_t*)stack_pop(&program_stack); 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    stack_push(&program_stack, &a);
                    stack_push(&program_stack, &b); 
                    break;
                case token_add: 
                    a=*(uint64_t*)stack_pop(&program_stack); 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a+=b; 
                    stack_push(&program_stack, &a);
                    break;
                case token_sub: 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack); 
                    a-=b; 
                    stack_push(&program_stack, &a);
                    break;
                case token_mul: 
                    a=*(uint64_t*)stack_pop(&program_stack); 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a*=b; 
                    stack_push(&program_stack, &a);
                    break;
                case token_div: 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack); 
                    result= a%b; 
                    stack_push(&program_stack, &result);
                    result= a/b; 
                    stack_push(&program_stack, &result);
                    break;
                case token_syscall: 
                    syscall();
                    break;
                case token_print_stack: stack_print(&program_stack, "%lu\n"); break;
                case token_rts: PC=*(uint64_t*)stack_pop(&subroutine_stack); break;
                case token_and:
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a & b;
                    stack_push(&program_stack, &result);
                break;
                case token_or: 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a | b;
                    stack_push(&program_stack, &result);    
                break;
                case token_not:
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a^0xFFFFFFFF;
                    stack_push(&program_stack, &result);
                break;
                case token_shl: 
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a << b;
                    stack_push(&program_stack, &result);   
                break;
                case token_shr:
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    result = a >> b;
                    stack_push(&program_stack, &result);   
                break;
                case token_lb:
                    b=*(uint64_t*)stack_pop(&program_stack);
                    memcpy(&lbval, program.instructions+b, sizeof(uint64_t));
                    stack_push(&program_stack, &lbval);
                break;
                case token_sb:
                    b=*(uint64_t*)stack_pop(&program_stack); 
                    a=*(uint64_t*)stack_pop(&program_stack);
                    memcpy(program.instructions+b, &a, sizeof(uint64_t));   
                break;
            }
        };
        
        
    }
    
    
}