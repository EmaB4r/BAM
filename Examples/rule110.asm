.include "Examples/std/array.asm"
.include "Examples/std/syscalls.asm"
.def CELLS 50
.def RULE 110
@main
    // array initialization as 0
    push 0
    push CELLS
    push line
    jsr i_array
    
    // in CELLS - 2 i put a 1
    push 1
    push line
    push CELLS
    subi 2
    jsr w_array
    
    push CELLS
    subi 2
    push 0
    @_loop_iter
        //print the line
        push CELLS
        push line
        jsr pc_array
    
        //PATTERN initialization
        push line
        push 0
        jsr r_array
        shli 1
        push line
        push 1
        jsr r_array
        or // pattern = (line[0]<<1) | line[1];
        
        push CELLS
        subi 1
        push 0
        @calc_loop
            dup 1
            addi 1
            push line
            swap
            jsr r_array //returns line[i+1]
            roll 4
            roll 4
            roll 4
            shli 1
            andi 7
            or
            
            dup 1
            push RULE
            swap
            shr
            andi 1
            roll 3
            roll 3
            
            dup 1
            roll 4
            
            push line
            swap
            
            jsr w_array
            roll 3

            
            addi 1
            dup 2
        jlt calc_loop
        pop pop pop
        addi 1
        dup 2
    jlt _loop_iter
    
stop

@pc_array
    swap
    push 0
    @_pc_array_loop
        dup 3
        swap
        pop
        jsr r_array
        
        dup 1
        push 0
        jeq _trueif_conversion
        //_falseif_conversion    
            push '#'
            mul
            jmp _endif_conversion 
        @_trueif_conversion
            addi ' ' // 0 + ' '
        @_endif_conversion
        
        syscall0
    
        addi 1
        dup 2
    jlt _pc_array_loop
    push '\n'
    syscall0
    pop
    pop
    pop
rts

@line
.byte CELLS