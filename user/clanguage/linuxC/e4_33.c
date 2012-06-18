#include <stdio.h>
#include <string.h>

void reverse(char* p)
{
	if(*p=='\0')
		return;
	reverse(p+1);
	putchar(*p);
}

int main()
{
	reverse("hello,world!");
	putchar('\n');
	return 0;
}
