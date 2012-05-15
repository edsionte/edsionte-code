#include <stdio.h>
#include <string.h>

int main()
{
//	char *dst;
	char* src="hello";
	char a[3];
//	dst=a;

	puts(strcpy(a,src));
	puts(a);
	return 0;
}
