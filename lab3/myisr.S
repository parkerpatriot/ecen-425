
CPU 8086
align 2

isr_tick:
; save context
push bp
mov bp, sp
push ax
push bx
push cx
push dx
push si
push di
push es
push ds
sti ; re-enable interrupts before executing handler
call tickHandler
cli ; disable interrupts before restoring state
call signalEOI ; tell the PIC that we're done with the ISR
; restore context
pop ds
pop es
pop di
pop si
pop dx
pop cx
pop bx
pop ax
mov sp, bp
pop bp
iret ; execute the interrupt return command


isr_reset:
call resetHandler


isr_key:
; save context
push bp
mov bp, sp
push ax
push bx
push cx
push dx
push si
push di
push es
push ds
sti ; re-enable interrupts before executing handler
call keyHandler
cli ; disable interrupts before restoring state
call signalEOI ; tell the PIC that we're done with the ISR
; restore context
pop ds
pop es
pop di
pop si
pop dx
pop cx
pop bx
pop ax
mov sp, bp
pop bp
iret ; execute the interrupt return command

