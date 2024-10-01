# BAM
B4r Machine.

This is a stack-based assembly language designed for manipulating values on a stack and controlling program execution. The language supports arithmetic, logical operations, and system interaction through syscalls.  

Build the bytecode assembler and the VM interpreter with `./build`   
Then simply create a new `.basm` file, compile it by doing `./Basm new.basm` and run it with `./Bam b.out`

## Table of Contents 
- [Instructions with a Parameter](#instructions-with-a-parameter) 
- [Instructions without a Parameter](#instructions-without-a-parameter) 
- [Syscalls](#syscalls) 
- [Labels](#labels) 
- [Directives](#directives) 
- [Comments](#comments)

## Instruction Set:
### Instructions with a parameter
- **push** : pushes the given parameter onto the stack
  `push 5 //B[]T --> B[5]T`
- **roll** : Rotates the top N elements of the stack, moving the first element to the bottom and shifting the others up
  `roll 4 //B[5, 4, 3, 2, 1]T --> B[5, 1, 4, 3, 2]T`
- **dup** : duplicates the N top elements
  `dup 2 //B[5, 3, 22]T --> B[5, 3, 22, 3, 22]T` 
- **andi** : bitwise AND between top value of the stack and the parameter
  `andi 6 //B[13]T --> B[4]T`
- **ori** : bitwise OR between top value of the stack and the parameter
  `ori 3 //B[10]T --> B[11]T`
- **noti** : pushes bitwise NOT of parameter
  `noti 255 //B[]T --> B[0xFFFFFF00]T`
- **shli** : shifts the top value of the stack left  
  `shli 2 //B[32]T --> B[8]T`
- **shri** : shifts the top value of the stack right
  `shri 2 //B[8]T --> B[32]T`
- **addi** : adds the parameter to the top of the stack
  `addi 5 //B[12]T --> B[17]T`
- **subi** : subs the parameter to the top of the stack
  `subi 5 //B[12]T --> B[7]T`
- **muli** : TODO
- **divi** : TODO
- **jmp** : jumps to the given address
  `@lable`
  `jmp lable`
- **jsr** : jumps to the given address and saves the return address
- **jeq** : pops 2 values from the stack and jumps to the given address, if they're equal
- **jne** : pops 2 values from the stack and jumps to the given address, if they're not equal
- **jlt** : pops 2 values from the stack and jumps to the given address, if the one on the top is less than the one under
- **jgt** : pops 2 values from the stack and jumps to the given address, if the one on the top is greater than the one under
- **jle** : pops 2 values from the stack and jumps to the given address, if the one on the top is less than or equal the one under
- **jge** : pops 2 values from the stack and jumps to the given address, if the one on the top is greater than or equal the one under
  ```
  @lable
  push 5
  dup 1
  addi 4
  jgt lable // 9 > 5 -> true
  ```

### Instructions without a parameter
- **pop** : pops the top element of the stack
- **sb** : pops an addres, then pops a value and stores the value at the address
- **lb** : Pops an address and pushes the value stored at that address
- **and** : pops 2 values from the stack and pushes the bitwise AND
- **or** : pops 2 values from the stack and pushes the bitwise OR
- **not** : pops a value and pushes the bitwise NOT
- **shl** : pops N and a value and pushes `value << N`
- **shr** : pops N and a value and pushes `value >> N`
- **stop** : ends program execution
- **rts** : returns from a subroutine
- **syscall** : [explained here](#syscalls)
- **print_stack** : instruction useful for debugginf; prints the whole stack to terminal without modifying it
- **swap** : swaps the 2 top elements
  `swap // B[2, 3]T --> B[3, 2]T`
- **add** : pops 2 elements from the stack and pushes their sum
- **sub** : pops 2 elements from the stack and pushes their subtraction
  `sub // B[10, 4]T --> B[6]T`
- **mul** : pops 2 elements from the stack and pushes their product
- **div** : pops 2 elements from the stack and pushes their division (top: a/b; bottom: a%b)
  `div // B[19, 4]T --> B[3, 4]T`


### Syscalls
syscalls expect a syscall code on top of the stack and are defined by that.
  - **0 (print char)** : pops a value from the stack and prints it as a char
  - **2 (print uint)** : pops a value from the stack and prints it as an unsigned int
  - TODO:{
	  1 (print string): pops a value from the stack (string address) and uses it to print an asciiz string
	  3 (print int): pops a value from the stack and prints it as an int
	  4 (read char): ...
	}

### Lables
```
@lable_name : defines a lable  
```

### Directives
- **.byte N** : reserves `N` bytes as variables/array (embedded into the program and initialised as 0)
- **.asciiz** : embeds into the program a null terminated ascii string `HELLO` -> `HELLO0`
- **.include path/to/file.basm** : embeds a basm file into the program starting from that line  
- **.def varname num** : defines a precompiler variable as `varname` and assigns to it `num`


### Comments
```
// this is a SINGLE LINE comment
//this is too

/*  
  this is a Comment Block


  it spans between multiple lines
*/  
```

## TODO:
implement precompiler .ifdef and .endif like headerguards
support for macro definition
refactor all instructions that work on more than 1 value on the stack to expect polish notation