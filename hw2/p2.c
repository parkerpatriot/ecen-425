#include <stdio.h>
#include "stdint.h"

char global_array[6] = "hello";
char global_no_touch[10] = "untouched";


void print_array(char* array, int size){
	int i;
	for(i=0; i<size; i++){
		printf("Address: %p\t\t\t Value: %c\n", &array[i], array[i]);
	}
}

void write_to_array(char* array, char* string, int size){
	int i;
	for(i=0; i<size; i++){
		array[i] = string[i];
	}
}


int main() {

	char local_array[6] = "hello";
	char local_no_touch[10] = "untouched";


	printf("Global variable global_array address: %p\n", &global_array);
	print_array(global_array, 6);

	printf("Global variable global_no_touch address: %p\n", &global_no_touch);
	print_array(global_no_touch, 10);


	printf("Wrting over bounds of first global variable...\n");
	write_to_array(global_array, "hello  ", 8);
	printf("After...\n");
    printf("First global array:\n");
    print_array(global_array, 6);
    printf("Second global array:\n");
    print_array(global_no_touch, 10);

	printf("Local variable local_array address: %p\n", &local_array);
	print_array(local_array, 6);
	printf("Local variable local_no_touch address: %p\n", &local_no_touch);
	print_array(local_no_touch, 10);

	printf("Wrting over bounds of first local variable...\n");
	write_to_array(local_array, "hello               ", 6+12);
	printf("After...\n");
    printf("First local array:\n");
    print_array(local_array, 6);
    printf("Second local array:\n");
    print_array(local_no_touch, 10);

    return 0;
}
