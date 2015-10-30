// CC = cc	
// CFLAGS = -g -Aa
// OBJ = src1.o src2.o src3.o

// prog1:	$(OBJ)
// 	$(CC) $(CFLAGS) $(OBJ) -o prog1

// src1.o:	ops.h def.h src1.c

// src2.o:	ops.h def.h src2.c

// src3.o:	ops.h def2.h src3.c

// define the global variables that make will reference

define CC to be 'cc' so we use the default compiler
define CFLAGS to be '-g -Aa' which will set the compiler flags
define OBJ to be 'src1.o src2.o src3.o'

// check all dependencies for prog1

if(src1.o is missing || ops.h, def.h, or src1.c have been modified){
	// execute 'cc -c src1.c -o src1.o'
	!! create/re-create src1.o ...
}
if(src2.o is missing || ops.h, def.h, or src2.c have been modified){
	// execute 'cc -c src2.c -o src2.o'
	!! create/re-create src2.o ...
}
if(src3.o is missing || ops.h, def3.h, or src3.c have been modified){
	// execute 'cc -c src3.c -o src3.o'
	!! create/re-create src3.o ...
}

// invoke the compiler to use the linker to create prog1
when(all the above dependencies have been checked){
	// execute 'cc -g -Aa src1.0 src2.o src3.o -o prog1'
	!! invoke the linker to create the final binary prog1 	
}
