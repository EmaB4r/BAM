
// TODO: implement also some 'get' routine to help get more bytes
// like a get(N) for getting N elements in a row, beeing chars or numers
@printc
    dup 1
    push 0
    syscall
rts

@printu
    dup 1
    push 2
    syscall
rts

@printcln
    dup 1
    push 0
    syscall
    push '\n'
    push 0
    syscall
rts

@printuln
    dup 1
    push 2
    syscall
    push '\n'
    push 0
    syscall
rts