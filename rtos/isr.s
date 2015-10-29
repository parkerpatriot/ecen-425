isr_reset:
	call YKResetHandler

isr_keypress:
	push	bp
	push	si
	push	di
	push	ds
	push	es
	push	dx
	push	cx
	push	bx
	push	ax
	call	YKEnterISR
	sti
	call	YKKeypressHandler
	cli
	call 	signalEOI ; tell the PIC that we're done with the ISR
	call	YKExitISR	
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

isr_tick:
	push	bp
	push	si	
	push	di
	push	ds
	push	es
	push	dx
	push	cx
	push	bx
	push	ax
	call	YKEnterISR
	;sti tick should never be interrupted
	call	YKTickHandler
	cli
	call 	signalEOI ; tell the PIC that we're done with the ISR
	call	YKExitISR	
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

