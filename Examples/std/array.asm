.def BYTESZ 8

// TODO: make the calling convention more consistent like:
// stack top [optional value, index, array address] stack bottom
// or for c_array like
// stack top [arr_size, arr1 addr, arr2 addr] stack bottom


// read array
// given an index and an array addr, puts on the stack array[index]
// expects, from top to bottom, the array index and array address
@r_array
    push BYTESZ
    mul
    add
    lb
rts

//write array
//given an index, an array addr and the value to store, does array[index] = value
//expects, from top to bottom, the array index, the array address and the value to write
@w_array
    push BYTESZ
    mul
    add
    sb
rts

//init array
//given the array address, the array size and the value, stores the value in all array's cells 
//expects, from top to bottom, the array address, its size and the value store
@i_array
    swap
    push 0
    @_i_array_loop
        dup 4
        swap
        pop
        jsr w_array
        addi 1
        dup 2
    jlt _i_array_loop
    pop
    pop
    pop
    pop
rts

//copy array
//given an address to a source array(src), a destination (dst) and src size, copies src into dst 
//requires, from top to bottom, src address, dst address, src size
@c_array
    roll 3
    roll 3
    push 0
    @_c_array_loop
        dup 4
        swap
        pop
        dup 1
        roll 3
        jsr r_array
        roll 3
        jsr w_array
        
        addi 1
        dup 2
    jlt _c_array_loop
rts



//expects, from top to bottom,the array address and its size
@p_array
    swap
    push 0
    @_p_array_loop
        dup 3
        swap
        pop
        jsr r_array
        jsr _ar_printt_elem
    
        addi 1
        dup 2
    jlt _p_array_loop
    push '\n'
    push 0
    syscall
    pop
    pop
    pop
rts


//expects the number to print
@_ar_printt_elem
    push 2
    syscall
    push ' '
    push 0
    syscall
rts