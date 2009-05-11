#include <stdio.h>


int main(void) {
	FILE * f;
	f = fopen("test.log", "a");
	fprintf(f, "hello, world\n");
	fclose(f);
	return 0;
}
