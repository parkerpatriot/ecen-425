; Generated by c86 (BYU-NASM) 5.1 (beta) from yakc.i
	CPU	8086
	ALIGN	2
	jmp	main	; Jump to program start
	ALIGN	2
YKIdleTask:
	; >>>>> Line:	33
	; >>>>> void YKIdleTask(void){ 
	jmp	L_yakc_1
L_yakc_2:
L_yakc_3:
	; >>>>> Line:	34
	; >>>>> while(1); 
L_yakc_4:
	jmp	L_yakc_3
L_yakc_5:
	mov	sp, bp
	pop	bp
	ret
L_yakc_1:
	push	bp
	mov	bp, sp
	jmp	L_yakc_2
	ALIGN	2
YKInitialize:
	; >>>>> Line:	37
	; >>>>> void YKInitialize(void){ 
	jmp	L_yakc_7
L_yakc_8:
	; >>>>> Line:	38
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	39
	; >>>>> YKsave = 0; 
	mov	word [YKsave], 0
	; >>>>> Line:	40
	; >>>>> YKrestore = 0; 
	mov	word [YKrestore], 0
	; >>>>> Line:	41
	; >>>>> YKTickCount = 0; 
	mov	word [YKTickCount], 0
	; >>>>> Line:	42
	; >>>>> YKCtxSwCount =  
	mov	word [YKCtxSwCount], 0
	; >>>>> Line:	43
	; >>>>> YKIdleCount = 0; 
	mov	word [YKIdleCount], 0
	; >>>>> Line:	44
	; >>>>> YKRunFlag = 0; 
	mov	word [YKRunFlag], 0
	; >>>>> Line:	45
	; >>>>> YKtaskCount = 0; 
	mov	word [YKtaskCount], 0
	; >>>>> Line:	46
	; >>>>> readyHead = 0; 
	mov	word [readyHead], 0
	; >>>>> Line:	47
	; >>>>> readyTail = 0; 
	mov	word [readyTail], 0
	; >>>>> Line:	48
	; >>>>> blockedHead = 0; 
	mov	word [blockedHead], 0
	; >>>>> Line:	49
	; >>>>> blockedTail = 0; 
	mov	word [blockedTail], 0
	; >>>>> Line:	50
	; >>>>> YKRunningTask = 0; 
	mov	word [YKRunningTask], 0
	; >>>>> Line:	51
	; >>>>> YKIsrDepth = 0; 
	mov	word [YKIsrDepth], 0
	; >>>>> Line:	52
	; >>>>> YKNewTask(&YKIdleTask, &YKIdleStk[256], 100); 
	mov	al, 100
	push	ax
	mov	ax, (YKIdleStk+512)
	push	ax
	mov	ax, YKIdleTask
	push	ax
	call	YKNewTask
	add	sp, 6
	mov	sp, bp
	pop	bp
	ret
L_yakc_7:
	push	bp
	mov	bp, sp
	jmp	L_yakc_8
	ALIGN	2
YKNewTask:
	; >>>>> Line:	55
	; >>>>> void YKNewTask(void (*task)(void), int *taskStack, unsigned char priority){ 
	jmp	L_yakc_10
L_yakc_11:
	; >>>>> Line:	59
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	61
	; >>>>> tempSP = taskStack-1; 
	mov	ax, word [bp+6]
	sub	ax, 2
	mov	word [bp-4], ax
	; >>>>> Line:	62
	; >>>>> *tempSP = 512; 
	mov	si, word [bp-4]
	mov	word [si], 512
	; >>>>> Line:	63
	; >>>>> tempSP = tempSP-1; 
	mov	ax, word [bp-4]
	sub	ax, 2
	mov	word [bp-4], ax
	; >>>>> Line:	64
	; >>>>> *tempSP = 0; 
	mov	si, word [bp-4]
	mov	word [si], 0
	; >>>>> Line:	65
	; >>>>> tempSP = tempSP-1; 
	mov	ax, word [bp-4]
	sub	ax, 2
	mov	word [bp-4], ax
	; >>>>> Line:	66
	; >>>>> *tempSP = task; 
	mov	si, word [bp-4]
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	67
	; >>>>> tempSP = tempSP-1; 
	mov	ax, word [bp-4]
	sub	ax, 2
	mov	word [bp-4], ax
	; >>>>> Line:	68
	; >>>>> *tempSP = tempSP; 
	mov	si, word [bp-4]
	mov	ax, word [bp-4]
	mov	word [si], ax
	; >>>>> Line:	69
	; >>>>> tempSP = tem 
	mov	ax, word [bp-4]
	sub	ax, 2
	mov	word [bp-4], ax
	; >>>>> Line:	70
	; >>>>> for(k = 0; k <=7; k++){ 
	mov	word [bp-6], 0
	jmp	L_yakc_13
L_yakc_12:
	; >>>>> Line:	71
	; >>>>> *tempSP = 0; 
	mov	si, word [bp-4]
	mov	word [si], 0
	; >>>>> Line:	72
	; >>>>> tempSP = tempSP-1; 
	mov	ax, word [bp-4]
	sub	ax, 2
	mov	word [bp-4], ax
L_yakc_15:
	inc	word [bp-6]
L_yakc_13:
	cmp	word [bp-6], 7
	jle	L_yakc_12
L_yakc_14:
	; >>>>> Line:	76
	; >>>>> tempTask = &YKtasks[YKtaskCount]; 
	mov	ax, word [YKtaskCount]
	mov	cx, 10
	imul	cx
	add	ax, YKtasks
	mov	word [bp-2], ax
	; >>>>> Line:	77
	; >>>>> tempTask->taskSP = tempSP+1; 
	mov	ax, word [bp-4]
	add	ax, 2
	mov	si, word [bp-2]
	mov	word [si], ax
	; >>>>> Line:	78
	; >>>>> tempTask->taskPriority = priority; 
	mov	si, word [bp-2]
	add	si, 2
	mov	al, byte [bp+8]
	mov	byte [si], al
	; >>>>> Line:	79
	; >>>>> tempTask->taskDelay = 0; 
	mov	si, word [bp-2]
	add	si, 4
	mov	word [si], 0
	; >>>>> Line:	81
	; >>>>> YKtaskCount++; 
	inc	word [YKtaskCount]
	; >>>>> Line:	83
	; >>>>> YKinsertReady(tempTask); 
	push	word [bp-2]
	call	YKinsertReady
	add	sp, 2
	; >>>>> Line:	84
	; >>>>> printStack(tempTask); 
	push	word [bp-2]
	call	printStack
	add	sp, 2
	; >>>>> Line:	85
	; >>>>> printLists(); 
	call	printLists
	; >>>>> Line:	86
	; >>>>> if (YKRunFlag == 1){ 
	cmp	word [YKRunFlag], 1
	jne	L_yakc_16
	; >>>>> Line:	87
	; >>>>> YKScheduler(1); 
	mov	ax, 1
	push	ax
	call	YKScheduler
	add	sp, 2
L_yakc_16:
	; >>>>> Line:	89
	; >>>>> YKExitMutex(); 
	call	YKExitMutex
	mov	sp, bp
	pop	bp
	ret
L_yakc_10:
	push	bp
	mov	bp, sp
	sub	sp, 6
	jmp	L_yakc_11
L_yakc_18:
	DB	"YKRun Failed: readyHead is Null",0
	ALIGN	2
YKRun:
	; >>>>> Line:	92
	; >>>>> void YKRun(void) { 
	jmp	L_yakc_19
L_yakc_20:
	; >>>>> Line:	93
	; >>>>> if (readyHead != 0){ 
	mov	ax, word [readyHead]
	test	ax, ax
	je	L_yakc_21
	; >>>>> Line:	94
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	95
	; >>>>> YKRunFlag = 1; 
	mov	word [YKRunFlag], 1
	; >>>>> Line:	96
	; >>>>> YKrestore = readyHead->taskSP; 
	mov	si, word [readyHead]
	mov	ax, word [si]
	mov	word [YKrestore], ax
	; >>>>> Line:	97
	; >>>>> YKRunningTask = readyHead; 
	mov	ax, word [readyHead]
	mov	word [YKRunningTask], ax
	; >>>>> Line:	98
	; >>>>> YKDispatcher(0); 
	xor	ax, ax
	push	ax
	call	YKDispatcher
	add	sp, 2
	jmp	L_yakc_22
L_yakc_21:
	; >>>>> Line:	101
	; >>>>> printString("YKRun Failed: readyHead is Null"); 
	mov	ax, L_yakc_18
	push	ax
	call	printString
	add	sp, 2
L_yakc_22:
	mov	sp, bp
	pop	bp
	ret
L_yakc_19:
	push	bp
	mov	bp, sp
	jmp	L_yakc_20
	ALIGN	2
YKDelayTask:
	; >>>>> Line:	105
	; >>>>> void YKDelayTask(unsigned count){ 
	jmp	L_yakc_24
L_yakc_25:
	; >>>>> Line:	107
	; >>>>> if (count != 0){ 
	mov	ax, word [bp+4]
	test	ax, ax
	je	L_yakc_26
	; >>>>> Line:	108
	; >>>>> item = YKRunningTask; 
	mov	ax, word [YKRunningTask]
	mov	word [bp-2], ax
	; >>>>> Line:	109
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	110
	; >>>>> item->taskDelay = count; 
	mov	si, word [bp-2]
	add	si, 4
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	111
	; >>>>> YKremoveReady(item); 
	push	word [bp-2]
	call	YKremoveReady
	add	sp, 2
	; >>>>> Line:	112
	; >>>>> YKinsertBlocked(item); 
	push	word [bp-2]
	call	YKinsertBlocked
	add	sp, 2
	; >>>>> Line:	113
	; >>>>> YKExitMutex(); 
	call	YKExitMutex
	; >>>>> Line:	114
	; >>>>> YKScheduler(1); 
	mov	ax, 1
	push	ax
	call	YKScheduler
	add	sp, 2
L_yakc_26:
	mov	sp, bp
	pop	bp
	ret
L_yakc_24:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_25
	ALIGN	2
YKExitISR:
	; >>>>> Line:	118
	; >>>>> void YKExitISR(void){ 
	jmp	L_yakc_28
L_yakc_29:
	; >>>>> Line:	119
	; >>>>> YKIsrDepth--; 
	dec	word [YKIsrDepth]
	; >>>>> Line:	120
	; >>>>> if(YKIsrDepth == 0){ 
	mov	ax, word [YKIsrDepth]
	test	ax, ax
	jne	L_yakc_30
	; >>>>> Line:	121
	; >>>>> YKScheduler(0); 
	xor	ax, ax
	push	ax
	call	YKScheduler
	add	sp, 2
L_yakc_30:
	mov	sp, bp
	pop	bp
	ret
L_yakc_28:
	push	bp
	mov	bp, sp
	jmp	L_yakc_29
	ALIGN	2
YKEnterISR:
	; >>>>> Line:	125
	; >>>>> void YKEnterISR(void){ 
	jmp	L_yakc_32
L_yakc_33:
	; >>>>> Line:	126
	; >>>>> YKIsrDepth++; 
	inc	word [YKIsrDepth]
	; >>>>> Line:	127
	; >>>>> if (YKIsrDepth == 1){ 
	cmp	word [YKIsrDepth], 1
	jne	L_yakc_34
	; >>>>> Line:	128
	; >>>>> YKsave = YKRunningTask->taskSP; 
	mov	si, word [YKRunningTask]
	mov	ax, word [si]
	mov	word [YKsave], ax
	; >>>>> Line:	129
	; >>>>> YKsaveSP(); 
	call	YKsaveSP
L_yakc_34:
	mov	sp, bp
	pop	bp
	ret
L_yakc_32:
	push	bp
	mov	bp, sp
	jmp	L_yakc_33
L_yakc_36:
	DB	"Context Switch",0xA,0xD,0
	ALIGN	2
YKScheduler:
	; >>>>> Line:	133
	; >>>>> YKEnterMutex(); 
	jmp	L_yakc_37
L_yakc_38:
	; >>>>> Line:	134
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	135
	; >>>>> if (YKRunningTask != readyHead){ 
	mov	ax, word [readyHead]
	cmp	ax, word [YKRunningTask]
	je	L_yakc_39
	; >>>>> Line:	136
	; >>>>> printString("Context Switch\n\r"); 
	mov	ax, L_yakc_36
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	137
	; >>>>> YKsave = YKRunningTask->taskSP; 
	mov	si, word [YKRunningTask]
	mov	ax, word [si]
	mov	word [YKsave], ax
	; >>>>> Line:	138
	; >>>>> YKrestore = readyHead->taskSP; 
	mov	si, word [readyHead]
	mov	ax, word [si]
	mov	word [YKrestore], ax
	; >>>>> Line:	139
	; >>>>> YKRunningTask = readyHead; 
	mov	ax, word [readyHead]
	mov	word [YKRunningTask], ax
	; >>>>> Line:	140
	; >>>>> YKCtxSwCount++; 
	inc	word [YKCtxSwCount]
	; >>>>> Line:	141
	; >>>>> YKDispatcher(saveContext); 
	push	word [bp+4]
	call	YKDispatcher
	add	sp, 2
L_yakc_39:
	; >>>>> Line:	143
	; >>>>> YKExitMutex(); 
	call	YKExitMutex
	mov	sp, bp
	pop	bp
	ret
L_yakc_37:
	push	bp
	mov	bp, sp
	jmp	L_yakc_38
L_yakc_41:
	DB	"TICK ",0
	ALIGN	2
YKTickHandler:
	; >>>>> Line:	146
	; >>>>> void YKTickHandler(void){ 
	jmp	L_yakc_42
L_yakc_43:
	; >>>>> Line:	148
	; >>>>> YKTickCount++; 
	inc	word [YKTickCount]
	; >>>>> Line:	149
	; >>>>> printString("TICK "); 
	mov	ax, L_yakc_41
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	150
	; >>>>> printInt(YKTickCount); 
	push	word [YKTickCount]
	call	printInt
	add	sp, 2
	; >>>>> Line:	151
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	152
	; >>>>> temp = blockedHead; 
	mov	ax, word [blockedHead]
	mov	word [bp-2], ax
	; >>>>> Line:	153
	; >>>>> while(temp != 0){ 
	jmp	L_yakc_45
L_yakc_44:
	; >>>>> Line:	154
	; >>>>> (temp->taskDelay)--; 
	mov	si, word [bp-2]
	add	si, 4
	dec	word [si]
	; >>>>> Line:	155
	; >>>>> if (temp->taskDelay <= 0){ 
	mov	si, word [bp-2]
	add	si, 4
	mov	ax, word [si]
	test	ax, ax
	jne	L_yakc_47
	; >>>>> Line:	156
	; >>>>> YKEnterMutex(); 
	call	YKEnterMutex
	; >>>>> Line:	157
	; >>>>> { 
	push	word [bp-2]
	call	YKremoveBlocked
	add	sp, 2
	; >>>>> Line:	158
	; >>>>> YKinsertReady(temp); 
	push	word [bp-2]
	call	YKinsertReady
	add	sp, 2
	; >>>>> Line:	159
	; >>>>> YKExitMutex(); 
	call	YKExitMutex
L_yakc_47:
	; >>>>> Line:	161
	; >>>>> temp = temp->next; 
	mov	si, word [bp-2]
	add	si, 6
	mov	ax, word [si]
	mov	word [bp-2], ax
L_yakc_45:
	mov	ax, word [bp-2]
	test	ax, ax
	jne	L_yakc_44
L_yakc_46:
	mov	sp, bp
	pop	bp
	ret
L_yakc_42:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_43
L_yakc_52:
	DB	") IGNORED",0
L_yakc_51:
	DB	"KEYPRESS (",0
L_yakc_50:
	DB	"DELAY COMPLETE",0
L_yakc_49:
	DB	"DELAY KEY PRESSED",0
	ALIGN	2
YKkeypress:
	; >>>>> Line:	165
	; >>>>> void YKkeypress(void){ 
	jmp	L_yakc_53
L_yakc_54:
	; >>>>> Line:	168
	; >>>>> if (KeyBuffer=='d'){ 
	mov	word [bp-2], 0
	mov	word [bp-4], 0
	; >>>>> Line:	168
	; >>>>> if (KeyBuffer=='d'){ 
	cmp	word [KeyBuffer], 100
	jne	L_yakc_55
	; >>>>> Line:	169
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	170
	; >>>>> printString("DELAY KEY PRESSED"); 
	mov	ax, L_yakc_49
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	171
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	172
	; >>>>> for (k=0; k<=5000; k++){ 
	mov	word [bp-2], 0
	jmp	L_yakc_57
L_yakc_56:
	; >>>>> Line:	173
	; >>>>> m++; 
	inc	word [bp-4]
L_yakc_59:
	inc	word [bp-2]
L_yakc_57:
	cmp	word [bp-2], 5000
	jle	L_yakc_56
L_yakc_58:
	; >>>>> Line:	175
	; >>>>> printString("DELAY COMPLETE"); 
	mov	ax, L_yakc_50
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	176
	; >>>>> printNewLine(); 
	call	printNewLine
	jmp	L_yakc_60
L_yakc_55:
	; >>>>> Line:	180
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	181
	; >>>>> printString("KEYPRESS ("); 
	mov	ax, L_yakc_51
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	182
	; >>>>> printChar(KeyBuffer); 
	push	word [KeyBuffer]
	call	printChar
	add	sp, 2
	; >>>>> Line:	183
	; >>>>> printString(") IGNORED"); 
	mov	ax, L_yakc_52
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	184
	; >>>>> printNewLine(); 
	call	printNewLine
L_yakc_60:
	mov	sp, bp
	pop	bp
	ret
L_yakc_53:
	push	bp
	mov	bp, sp
	sub	sp, 4
	jmp	L_yakc_54
	ALIGN	2
YKinsertReady:
	; >>>>> Line:	188
	; >>>>> void YKinsertReady(struct Task* item){ 
	jmp	L_yakc_62
L_yakc_63:
	; >>>>> Line:	190
	; >>>>> if (item != 0) { 
	mov	ax, word [bp+4]
	test	ax, ax
	je	L_yakc_64
	; >>>>> Line:	192
	; >>>>> if (readyHead == 0){ 
	mov	ax, word [readyHead]
	test	ax, ax
	jne	L_yakc_65
	; >>>>> Line:	193
	; >>>>> readyHead = item; 
	mov	ax, word [bp+4]
	mov	word [readyHead], ax
	; >>>>> Line:	194
	; >>>>> readyTail = item; 
	mov	word [readyTail], ax
	; >>>>> Line:	195
	; >>>>> item->next = 0; 
	mov	si, word [bp+4]
	add	si, 6
	mov	word [si], 0
	; >>>>> Line:	196
	; >>>>> item->prev = 0; 
	mov	si, word [bp+4]
	add	si, 8
	mov	word [si], 0
	jmp	L_yakc_66
L_yakc_65:
	; >>>>> Line:	200
	; >>>>> temp = readyHead; 
	mov	ax, word [readyHead]
	mov	word [bp-2], ax
	; >>>>> Line:	201
	; >>>>> while(temp != 0){ 
	jmp	L_yakc_68
L_yakc_67:
	; >>>>> Line:	203
	; >>>>> if ((temp->taskPriority) > (item->taskPriority)){ 
	mov	si, word [bp-2]
	add	si, 2
	mov	di, word [bp+4]
	add	di, 2
	mov	al, byte [di]
	cmp	al, byte [si]
	jae	L_yakc_70
	; >>>>> Line:	205
	; >>>>> if (temp->prev == 0){ 
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [si]
	test	ax, ax
	jne	L_yakc_71
	; >>>>> Line:	206
	; >>>>> item->next = temp; 
	mov	si, word [bp+4]
	add	si, 6
	mov	ax, word [bp-2]
	mov	word [si], ax
	; >>>>> Line:	207
	; >>>>> readyHead = item; 
	mov	ax, word [bp+4]
	mov	word [readyHead], ax
	; >>>>> Line:	208
	; >>>>> temp->prev = item; 
	mov	si, word [bp-2]
	add	si, 8
	mov	word [si], ax
	; >>>>> Line:	209
	; >>>>> item->prev = 0; 
	mov	si, word [bp+4]
	add	si, 8
	mov	word [si], 0
	jmp	L_yakc_72
L_yakc_71:
	; >>>>> Line:	213
	; >>>>> (temp->prev)->next = item; 
	mov	si, word [bp-2]
	add	si, 8
	mov	si, word [si]
	add	si, 6
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	214
	; >>>>> item->prev = temp->prev; 
	mov	si, word [bp-2]
	add	si, 8
	mov	di, word [bp+4]
	add	di, 8
	mov	ax, word [si]
	mov	word [di], ax
	; >>>>> Line:	215
	; >>>>> temp->prev = item; 
	mov	si, word [bp-2]
	add	si, 8
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	216
	; >>>>> item->next = temp; 
	mov	si, word [bp+4]
	add	si, 6
	mov	ax, word [bp-2]
	mov	word [si], ax
L_yakc_72:
	; >>>>> Line:	218
	; >>>>> return; 
	jmp	L_yakc_73
L_yakc_70:
	; >>>>> Line:	220
	; >>>>> temp = temp->next; 
	mov	si, word [bp-2]
	add	si, 6
	mov	ax, word [si]
	mov	word [bp-2], ax
L_yakc_68:
	mov	ax, word [bp-2]
	test	ax, ax
	jne	L_yakc_67
L_yakc_69:
	; >>>>> Line:	223
	; >>>>> readyTail->next = item; 
	mov	si, word [readyTail]
	add	si, 6
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	224
	; >>>>> item->prev = readyTail; 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [readyTail]
	mov	word [si], ax
	; >>>>> Line:	225
	; >>>>> readyTail = item; 
	mov	ax, word [bp+4]
	mov	word [readyTail], ax
	; >>>>> Line:	226
	; >>>>> item->next = 0; 
	mov	si, word [bp+4]
	add	si, 6
	mov	word [si], 0
L_yakc_66:
L_yakc_64:
L_yakc_73:
	mov	sp, bp
	pop	bp
	ret
L_yakc_62:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_63
	ALIGN	2
YKremoveReady:
	; >>>>> Line:	231
	; >>>>> void YKremoveReady (struct Task* item){ 
	jmp	L_yakc_75
L_yakc_76:
	; >>>>> Line:	232
	; >>>>> if (item != 0) { 
	mov	ax, word [bp+4]
	test	ax, ax
	je	L_yakc_77
	; >>>>> Line:	233
	; >>>>> if (item->prev != 0) { 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [si]
	test	ax, ax
	je	L_yakc_78
	; >>>>> Line:	234
	; >>>>> (item->prev)->next = item->next; 
	mov	si, word [bp+4]
	add	si, 6
	mov	di, word [bp+4]
	add	di, 8
	mov	di, word [di]
	add	di, 6
	mov	ax, word [si]
	mov	word [di], ax
	jmp	L_yakc_79
L_yakc_78:
	; >>>>> Line:	237
	; >>>>> readyHead = item->next; 
	mov	si, word [bp+4]
	add	si, 6
	mov	ax, word [si]
	mov	word [readyHead], ax
L_yakc_79:
	; >>>>> Line:	239
	; >>>>> if (item->next != 0) { 
	mov	si, word [bp+4]
	add	si, 6
	mov	ax, word [si]
	test	ax, ax
	je	L_yakc_80
	; >>>>> Line:	240
	; >>>>> (item->next)->prev = item->prev; 
	mov	si, word [bp+4]
	add	si, 8
	mov	di, word [bp+4]
	add	di, 6
	mov	di, word [di]
	add	di, 8
	mov	ax, word [si]
	mov	word [di], ax
	jmp	L_yakc_81
L_yakc_80:
	; >>>>> Line:	243
	; >>>>> readyTail = item->prev; 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [si]
	mov	word [readyTail], ax
L_yakc_81:
L_yakc_77:
	mov	sp, bp
	pop	bp
	ret
L_yakc_75:
	push	bp
	mov	bp, sp
	jmp	L_yakc_76
	ALIGN	2
YKinsertBlocked:
	; >>>>> Line:	248
	; >>>>> void YKinsertBlocked(struct Task* item){ 
	jmp	L_yakc_83
L_yakc_84:
	; >>>>> Line:	249
	; >>>>> if (item != 0) { 
	mov	ax, word [bp+4]
	test	ax, ax
	je	L_yakc_85
	; >>>>> Line:	250
	; >>>>> if (blockedTail == 0){ 
	mov	ax, word [blockedTail]
	test	ax, ax
	jne	L_yakc_86
	; >>>>> Line:	251
	; >>>>> * item){ 
	mov	ax, word [bp+4]
	mov	word [blockedHead], ax
	; >>>>> Line:	252
	; >>>>> blockedTail = item; 
	mov	word [blockedTail], ax
	; >>>>> Line:	253
	; >>>>> item->next = 0; 
	mov	si, word [bp+4]
	add	si, 6
	mov	word [si], 0
	; >>>>> Line:	254
	; >>>>> item->prev = 0; 
	mov	si, word [bp+4]
	add	si, 8
	mov	word [si], 0
	jmp	L_yakc_87
L_yakc_86:
	; >>>>> Line:	257
	; >>>>> blockedTail->next = item; 
	mov	si, word [blockedTail]
	add	si, 6
	mov	ax, word [bp+4]
	mov	word [si], ax
	; >>>>> Line:	258
	; >>>>> item->prev = blockedTail; 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [blockedTail]
	mov	word [si], ax
	; >>>>> Line:	259
	; >>>>> blockedTail = item; 
	mov	ax, word [bp+4]
	mov	word [blockedTail], ax
	; >>>>> Line:	260
	; >>>>> item->next = 0; 
	mov	si, word [bp+4]
	add	si, 6
	mov	word [si], 0
L_yakc_87:
L_yakc_85:
	mov	sp, bp
	pop	bp
	ret
L_yakc_83:
	push	bp
	mov	bp, sp
	jmp	L_yakc_84
	ALIGN	2
YKremoveBlocked:
	; >>>>> Line:	265
	; >>>>> void YKremoveBlocked(struct Task* item){ 
	jmp	L_yakc_89
L_yakc_90:
	; >>>>> Line:	266
	; >>>>> if (item != 0) { 
	mov	ax, word [bp+4]
	test	ax, ax
	je	L_yakc_91
	; >>>>> Line:	267
	; >>>>> if (item->prev != 0) { 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [si]
	test	ax, ax
	je	L_yakc_92
	; >>>>> Line:	268
	; >>>>> (item->prev)->next = item->next; 
	mov	si, word [bp+4]
	add	si, 6
	mov	di, word [bp+4]
	add	di, 8
	mov	di, word [di]
	add	di, 6
	mov	ax, word [si]
	mov	word [di], ax
	jmp	L_yakc_93
L_yakc_92:
	; >>>>> Line:	271
	; >>>>> blockedHead = item->next; 
	mov	si, word [bp+4]
	add	si, 6
	mov	ax, word [si]
	mov	word [blockedHead], ax
L_yakc_93:
	; >>>>> Line:	273
	; >>>>> if (item->next != 0) { 
	mov	si, word [bp+4]
	add	si, 6
	mov	ax, word [si]
	test	ax, ax
	je	L_yakc_94
	; >>>>> Line:	274
	; >>>>> (item->next)->prev = item->prev; 
	mov	si, word [bp+4]
	add	si, 8
	mov	di, word [bp+4]
	add	di, 6
	mov	di, word [di]
	add	di, 8
	mov	ax, word [si]
	mov	word [di], ax
	jmp	L_yakc_95
L_yakc_94:
	; >>>>> Line:	277
	; >>>>> blockedTail = item->prev; 
	mov	si, word [bp+4]
	add	si, 8
	mov	ax, word [si]
	mov	word [blockedTail], ax
L_yakc_95:
L_yakc_91:
	mov	sp, bp
	pop	bp
	ret
L_yakc_89:
	push	bp
	mov	bp, sp
	jmp	L_yakc_90
L_yakc_100:
	DB	0xA,0xD,"PrintStack: item is NULL",0
L_yakc_99:
	DB	"]",0xA,0xD,0
L_yakc_98:
	DB	": [",0
L_yakc_97:
	DB	0xA,0xD,"Printing Stack:",0xA,0xD,0
	ALIGN	2
printStack:
	; >>>>> Line:	282
	; >>>>> void printStack(struct Task* item){ 
	jmp	L_yakc_101
L_yakc_102:
	; >>>>> Line:	285
	; >>>>> k = 0; 
	mov	word [bp-2], 0
	; >>>>> Line:	286
	; >>>>> if (item != 0){ 
	mov	ax, word [bp+4]
	test	ax, ax
	je	L_yakc_103
	; >>>>> Line:	287
	; >>>>> tempSP = item->taskSP; 
	mov	si, word [bp+4]
	mov	ax, word [si]
	mov	word [bp-4], ax
	; >>>>> Line:	288
	; >>>>> printString("\n\rPrinting Stack:\n\r"); 
	mov	ax, L_yakc_97
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	289
	; >>>>> for (k; k<12; k++){ 
	jmp	L_yakc_105
L_yakc_104:
	; >>>>> Line:	290
	; >>>>> printWord(tempSP); 
	push	word [bp-4]
	call	printWord
	add	sp, 2
	; >>>>> Line:	291
	; >>>>> printString(": ["); 
	mov	ax, L_yakc_98
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	292
	; >>>>> printWord(*tempSP); 
	mov	si, word [bp-4]
	push	word [si]
	call	printWord
	add	sp, 2
	; >>>>> Line:	293
	; >>>>> printString("]\n\r"); 
	mov	ax, L_yakc_99
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	294
	; >>>>> tempSP = tempSP+1; 
	mov	ax, word [bp-4]
	add	ax, 2
	mov	word [bp-4], ax
L_yakc_107:
	inc	word [bp-2]
L_yakc_105:
	cmp	word [bp-2], 12
	jl	L_yakc_104
L_yakc_106:
	jmp	L_yakc_108
L_yakc_103:
	; >>>>> Line:	298
	; >>>>> printString("\n\rPrintStack: item is NULL"); 
	mov	ax, L_yakc_100
	push	ax
	call	printString
	add	sp, 2
L_yakc_108:
	mov	sp, bp
	pop	bp
	ret
L_yakc_101:
	push	bp
	mov	bp, sp
	sub	sp, 4
	jmp	L_yakc_102
L_yakc_114:
	DB	"Blocked List:",0xA,0xD,0
L_yakc_113:
	DB	"] ",0
L_yakc_112:
	DB	", ",0
L_yakc_111:
	DB	"[0x",0
L_yakc_110:
	DB	0xA,0xD,"Ready List:",0xA,0xD,0
	ALIGN	2
printLists:
	; >>>>> Line:	303
	; >>>>> void printLists(void){ 
	jmp	L_yakc_115
L_yakc_116:
	; >>>>> Line:	305
	; >>>>> printString("\n\rReady List:\n\r"); 
	mov	ax, L_yakc_110
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	306
	; >>>>> temp = readyHead; 
	mov	ax, word [readyHead]
	mov	word [bp-2], ax
	; >>>>> Line:	307
	; >>>>> while(temp != 0){ 
	jmp	L_yakc_118
L_yakc_117:
	; >>>>> Line:	308
	; >>>>> printString("[0x"); 
	mov	ax, L_yakc_111
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	309
	; >>>>> printByte(temp->taskPriority); 
	mov	si, word [bp-2]
	add	si, 2
	push	word [si]
	call	printByte
	add	sp, 2
	; >>>>> Line:	310
	; >>>>> printString(", "); 
	mov	ax, L_yakc_112
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	311
	; >>>>> p 
	mov	si, word [bp-2]
	add	si, 4
	push	word [si]
	call	printWord
	add	sp, 2
	; >>>>> Line:	312
	; >>>>> printString(", "); 
	mov	ax, L_yakc_112
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	313
	; >>>>> printWord(temp->taskSP); 
	mov	si, word [bp-2]
	push	word [si]
	call	printWord
	add	sp, 2
	; >>>>> Line:	314
	; >>>>> printString("] "); 
	mov	ax, L_yakc_113
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	315
	; >>>>> temp = temp->next; 
	mov	si, word [bp-2]
	add	si, 6
	mov	ax, word [si]
	mov	word [bp-2], ax
L_yakc_118:
	mov	ax, word [bp-2]
	test	ax, ax
	jne	L_yakc_117
L_yakc_119:
	; >>>>> Line:	317
	; >>>>> printNewLine(); 
	call	printNewLine
	; >>>>> Line:	318
	; >>>>> temp = blockedHead; 
	mov	ax, word [blockedHead]
	mov	word [bp-2], ax
	; >>>>> Line:	319
	; >>>>> printString("Blocked List:\n\r"); 
	mov	ax, L_yakc_114
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	320
	; >>>>> while(temp != 0){ 
	jmp	L_yakc_121
L_yakc_120:
	; >>>>> Line:	321
	; >>>>> printString("[0x"); 
	mov	ax, L_yakc_111
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	322
	; >>>>> printByte(temp->taskPriority); 
	mov	si, word [bp-2]
	add	si, 2
	push	word [si]
	call	printByte
	add	sp, 2
	; >>>>> Line:	323
	; >>>>> printString(", "); 
	mov	ax, L_yakc_112
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	324
	; >>>>> printWord(temp->taskDelay); 
	mov	si, word [bp-2]
	add	si, 4
	push	word [si]
	call	printWord
	add	sp, 2
	; >>>>> Line:	325
	; >>>>> printString(", "); 
	mov	ax, L_yakc_112
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	326
	; >>>>> printWord(temp->taskSP); 
	mov	si, word [bp-2]
	push	word [si]
	call	printWord
	add	sp, 2
	; >>>>> Line:	327
	; >>>>> printString("] "); 
	mov	ax, L_yakc_113
	push	ax
	call	printString
	add	sp, 2
	; >>>>> Line:	328
	; >>>>> temp = temp->next; 
	mov	si, word [bp-2]
	add	si, 6
	mov	ax, word [si]
	mov	word [bp-2], ax
L_yakc_121:
	mov	ax, word [bp-2]
	test	ax, ax
	jne	L_yakc_120
L_yakc_122:
	; >>>>> Line:	330
	; >>>>> printString("\n\r"); 
	mov	ax, (L_yakc_36+14)
	push	ax
	call	printString
	add	sp, 2
	mov	sp, bp
	pop	bp
	ret
L_yakc_115:
	push	bp
	mov	bp, sp
	push	cx
	jmp	L_yakc_116
	ALIGN	2
YKtaskCount:
	TIMES	2 db 0
YKRunFlag:
	TIMES	2 db 0
YKIsrDepth:
	TIMES	2 db 0
YKTickCount:
	TIMES	2 db 0
YKCtxSwCount:
	TIMES	2 db 0
YKIdleCount:
	TIMES	2 db 0
readyHead:
	TIMES	2 db 0
readyTail:
	TIMES	2 db 0
blockedHead:
	TIMES	2 db 0
blockedTail:
	TIMES	2 db 0
YKRunningTask:
	TIMES	2 db 0
YKsave:
	TIMES	2 db 0
YKrestore:
	TIMES	2 db 0
YKtasks:
	TIMES	100 db 0
YKIdleStk:
	TIMES	512 db 0
