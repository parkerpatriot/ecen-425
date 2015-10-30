


; Problem 4.a

mov al, 2
push ax
mov ax, 1
push ax
call MyFunction
add sp, 4

; Problem 4.b

[bp+4] ; argWord
[bp+6] ; argByte
[bp-2] ; localWord
[bp-3] ; localByte

; Problem 4.c

; All arguments are pushed onto the stack, the local variables are also pushed onto the stack but they are referenced from the new stack frame

