.ifndef IO_ASM
.def IO_ASM 1

.include "Examples/std/syscalls.asm"

// TODO: implement also some 'get' routine to help get more bytes
// like a get(N) for getting N elements in a row, beeing chars or numers
@printc
    dup 1
    syscall0
rts

@printu
    dup 1
    syscall2
rts

@printcln
    dup 1
    push 0
    syscall
    push '\n'
    syscall0
rts

@printuln
    dup 1
    syscall2
    push '\n'
    syscall0
rts

.endif