#####################################################################
# ECEn 425 Lab 3 Makefile

hw5.bin:	array.c
		cpp array.c array.i
		c86 -g array.i array.s
		cat clib.S array.s > final.s
		nasm final.s -o hw5.bin -l lab3.lst

clean:
		rm array.i array.s final.s lab3.lst hw5.bin
