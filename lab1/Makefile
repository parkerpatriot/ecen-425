######################################################################
# Sample EE425 Makefile

all: lab1.i lab1.s lab1final.s lab1final.bin

lab1.i:	lab1.c
		cpp lab1.c lab1.i				# Step 1, Preprocess

lab1.s:	lab1.i
		c86 -g lab1.i lab1.s 				# Step 2, Compile

lab1final.s: clib.s lab1asm.s lab1.s
		cat clib.s lab1asm.s lab1.s > lab1final.s 	# Step 3, Concatenate

lab1final.bin:	lab1final.s
		nasm lab1final.s -o lab1.bin -l lab1.lst 	# Step 4, Assemble

clean:	
		rm lab1.bin lab1.lst lab1final.s lab1.s lab1.i
