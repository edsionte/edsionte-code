#include <stdio.h>
#include <stdlib.h>

struct student
{
	char name[20];
	int age;
	char sex;
	char phone[16];
};

struct data
{
	char a;
	char b;
	char c;
	char d;
	char e;
};

struct data1
{
	char a;
	double b;
	char c;
};

int main()
{
	struct data p;
	struct data1 p2;
	printf("p=%d\n",sizeof(p));
	printf("p2=%d\n",sizeof(p2));
	return 0;
}
