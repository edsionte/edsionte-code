#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char str[]="hello,my ID is edsionte521!";
	char *p=str;
	char *q=&str[strlen(str)-1];
	char temp;

	while(p<q)
	{
		temp=*p;
		*p=*q;
		*q=temp;
		p++;
		q--;
	}

	puts(str);
	return 0;
}
