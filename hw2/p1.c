#include "stdint.h"
#include <stdio.h>


char strvar1[100];
char strvar2[100];
int a;
int b;

int main() {

	while(1){

		
		printf("Enter a: ");
		fgets (strvar1, 100, stdin);
		a = atoi(strvar1);

		printf("Enter b: ");
		fgets (strvar2, 100, stdin);
		b = atoi(strvar2);	


		// START OF CODE UNDER TEST
		if (a != b)
		    if (a < b)
		        printf("in the if!\n");
		else
		    printf("in the else!\n");

		// END OF CODE UNDER TEST

		// this else statmement is paried with
		// the first if statement: if(a != b)
		else
			printf("second else!\n");

		printf("end of program\n");

	}
}