@main

//N = num to calculate factorial
.def N 5
push N
jsr calculate_factorial
jsr printuln

.def M 3
push M
jsr calculate_factorial
jsr printuln

stop


//takes the number on top of the stack and calculates its factorial, putting it on the top
@calculate_factorial
    jsr printu
    push '!'
    jsr printc
    pop
    push ' '
    jsr printc
    pop
    push '='
    jsr printc
    pop
    push ' '
    jsr printc
    pop
    //check if the number is 0, if yes pushes 1 and returns
    dup 1
    push 0
    jeq num_is_z
    
    dup 1 // copy N to have on the top an iterator (from N to 1) and under it the current result
@_calc_fac
    //decrease the iterator
    subi 1
    
    //save it for later
    dup 1
    roll 3
    
    //calculate the next current result 
    mul
    
    //swap to have again on the top the iterator and under it the current result
    swap
    
    //check if iterator == 1
    dup 1
    push 1
    jne _calc_fac
    
    pop //remove the iterator
    jmp _end_factorial
    
    
@num_is_z
    pop
    push 1
@_end_factorial
rts

.include "Examples/std/io.asm"