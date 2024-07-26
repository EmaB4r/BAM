## BAM
B4r Machine.  
Set of assembler and interpreter for my own stack-oriented assembly language  
build code using `./build.sh`   

The assebler, `basm`, takes a `.basm` assembly file and converts it to bytecodes for the interpreter `bam`.  

# Instruction Set:
### Instructions with a parameter
```
push : pushes the given parameter onto the stack
roll : pops N values from the stack and pushes them back with the top one now at the bottom  
   | 1 |    | 2 |
   | 2 |    | 3 |
   | 3 | -> | 4 |
   | 4 |    | 5 |
   | 5 |    | 1 |  
jsr : sets PC to the given address and saves the return address
jmp : sets PC to the given address
jeq : pops 2 values from the stack and sets PC to the given address, if they're equal
jne : pops 2 values from the stack and sets PC to the given address, if they're not equal
token_jlt : pops 2 values from the stack and sets PC to the given address, if the one on the top is less than the one under
token_jgt : pops 2 values from the stack and sets PC to the given address, if the one on the top is greater than the one under
token_jle : pops 2 values from the stack and sets PC to the given address, if the one on the top is less than or equal the one under
token_jge : pops 2 values from the stack and sets PC to the given address, if the one on the top is greater than or equal the one under
```

### Instructions without a parameter
```
pop : pops the top element of the stack
dup : duplicates the top element
stop : ends program execution
rts : sets PC to the return address at the end of a subroutine
print : pops the top element of the stack and prints it out
print_stack : instruction useful for debug; prints the whole stack to terminal without modifying it
swap : swaps the 2 top elements
add : pops 2 elements from the stack and pushes their sum
sub : pops 2 elements from the stack and pushes their subtraction
mul : pops 2 elements from the stack and pushes their product
div : pops 2 elements from the stack and pushes their division (top: a/b; bottom: a%b)
```

### Metainstructions
```
@lable_name : defines a lable
.include filename.basm : includes a basm file starting from that line 
//COMMENT LINE
```

### Future adds:
support for macro definition  
block comments using `/* comment */`  
some sort of syscalls for inputting and outputting text or numbers  