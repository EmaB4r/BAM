# 20/02/2025
## CODE REFACTORING
- Lexer: now it lexes completely a file and stores every token in a list, instead of lexing in real time a file
- Parser: moved all the metainstructions to a new precompiler module
- Precompiler: new module able to define and replace macros, symbols and more

# 01/10/2024
## NEW INSTRUCTIONS!  
### With Immediates:
- **dup**
- andi
- ori
- noti
- shli
- shri
- addi
- subi 
- muli
- divi

### Without immediates:
- sb
- lb
- syscall
-  **~~dup~~**
- and
- or
- not
- shl
- shr
## NEW DIRECTIVES!
- .byte