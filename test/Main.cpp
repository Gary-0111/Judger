#include <stdio.h>

int array[256*1024*63];

int main() {
	int a, b;
	while(~scanf("%d%d", &a, &b))
		printf("%d\n", a+b);
	return 0;
}
