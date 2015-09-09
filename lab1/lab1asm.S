; Modify AsmFunction to perform the calculation gvar+(a*(b+c))/(d-e).
; Keep in mind the C declaration:
; int AsmFunction(int a, char b, char c, int d, int e);

	CPU	8086
	align	2

AsmFunction:
	; [bp+2]  -> ret
	; [bp+4]  -> a
	; [bp+6]  -> b
	; [bp+8]  -> c
	; [bp+10] -> d
	; [bp+12] -> e
	push bp
	mov bp, sp
	push bx
	; (b+c)
	mov	ax, [bp+6]
	add ax, [bp+8]
	; sign extend (for char arithmetic)
	cbw
	; a*(b+c)
	mov bx, [bp+4]
	mul bx
	; (d-e)
	mov bx, [bp+10]
	sub bx, [bp+12]
	; extend ax to double word for division
	cwd
	; (a*(b+c))/(d-e)
	idiv bx
	; gvar+(a*(b+c))/(d-e)
	add ax, [gvar]
	; reset the stack
	pop bx
	mov sp, bp
	pop bp
	ret

	

