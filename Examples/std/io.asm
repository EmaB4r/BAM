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