#define max(A,B)  ((A) > (B) ? (A) : (B))
#include <stdio.h>

int main(){
	int i=-1, j=-3;
	char a='a', b='b', c='c';

	printf("%d\n", max(i++, j++));

	return 0;
}