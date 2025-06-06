//FIBONACCI
.include "Examples/std/io.asm"
.def N 15
@main
    push 0 //iterator
    push 0 //first number
    jsr printuln
    push 1 //second number
    jsr printuln
    
@calc
//i start by saving the biggest value for the next iteration 
    dup 1
    roll 3
//sum the 2 current numbers
    add
    jsr printuln
    
//move the iterator to the top and increment it 
    roll 3
    roll 3
    push 1
    add

    dup 1
    roll 4 //moves the duped iterator to the bottom
    push N //number of iterations until stop
    jne calc
    stop