YKEnterMutex:
	cli
	ret
YKExitMutex:
	sti
	ret
YKsavecontext:
	sub	sp, 24
	mov	[YKsave], sp
	add	sp, 24
	sti
	pushf
	push	cs
	push	word[bp+2]
	push	word[bp]
	push	si
	push	di
	push	ds
	push	es
	push	dx
	push	cx
	push	bx
	push	ax  
	jmp	YKrestorecontext
YKDispatcher:
	cli
	push	bp
	mov	bp, sp
	push	ax
	mov	ax, [bp+4]
	cmp	ax, 1
	pop	ax
	je	YKsavecontext
YKrestorecontext:
	mov	sp, [YKrestore]
	pop	ax
	pop	bx
	pop	cx
	pop	dx
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp	
	iret
YKsaveSP:
	push 	bp
	mov 	bp, sp
	add	sp, 8
	mov	word [YKsave], sp  ;ISR saved context is two function calls away
	sub	sp, 8
	mov	sp, bp
	pop	bp
	ret
