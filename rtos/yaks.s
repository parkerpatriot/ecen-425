YKEnterMutex:
	cli
	ret
YKExitMutex:
	sti
	ret
YKSavecontext:
	pushf
	push	bx
	add		sp, 2
	mov		bx, sp
	or		word[bx], 0x0200
	sub		sp, 2
	pop		bx
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
	mov		bx, [YKsave]
	mov		[bx], sp
	jmp		YKRestoreContext
YKDispatcher:
	push	bp
	mov		bp, sp
	push	ax
	mov		ax, [bp+4] 	; get arg
	cmp		ax, 1		;save context?
	pop		ax
	je		YKSavecontext
YKRestoreContext:
	mov		sp, [YKrestore]
	pop		ax
	pop		bx
	pop		cx
	pop		dx
	pop		es
	pop		ds
	pop		di
	pop		si
	pop		bp	
	iret
YKSaveSP:
	push 	bp
	mov 	bp, sp
	add		sp, 8 	; two function calls away
	mov 	bx, [YKsave]
	mov 	[bx], sp
	sub		sp, 8 	; undo sp adjustment
	mov 	sp, bp
	pop		bp
	ret
