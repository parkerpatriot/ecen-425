#include "stdint.h"
#include "stdio.h"


void function1(int a, int b, char *string){
	int var;
	var = a + b;
	printf("int: %d\n", var);
	printf("string: %s\n", string);
}

int main(){

	const char *my_string = "my string";

	// mistmatch int instead of char*
	// function1(1, 2, 3);

	// mismatch char instead of int
	// function1('a', 5, 'this');

	// mismatch number of args
	// function1(1, 2, 'hello', 4);

	return 0;
}