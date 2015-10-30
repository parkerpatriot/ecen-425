; Seg:Off  |  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
; ----------|------------------------------------------------
; 0000:0000 | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
; 0000:0010 | 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
; 0000:0020 | 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
; 0000:0030 | 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F


; Initial register values
; AX:0000 BX:0002 CX:0000 DX:0000 SI:0001 DI:0000 BP:0002
; DS:0001 ES:0002 SS:0000 CS:0000 SP:FFFE IP:0100

mov	ax, [bx+2]		; ax -> 0x1514 
dec	word [bx+2]		; 0000:0014 -> 0x1513
mov	bh, [bx+3]		; bh -> 0x15
mov	cx, [bp+si-3]	; cx -> 0x1110
mov	dx, [es:bp+2]	; dx -> 0x2524

; Initial register values
; AX:0000 BX:0002 CX:0000 DX:0000 SI:0001 DI:0000 BP:0002
; DS:0001 ES:0002 SS:0000 CS:0000 SP:FFFE IP:0100

; ax -> 0x1514
; bx -> 0x1502
; cx -> 0x1110
; dx -> 0x2524