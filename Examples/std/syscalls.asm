.ifndef SYSCALLS
.def SYSCALLS 1

.macro syscall0
    push 0
    syscall
.endmacro

.macro syscall2
    push 2
    syscall
.endmacro

.endif