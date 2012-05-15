#include <stdio.h>

int get_sum(int n)
{
	int sum=0,i;

	for(i=0;i<n;i++)
		sum+=i;
	return sum;
}

int main()
{
	int i=10;
	printf("sum=%d\n",get_sum(i));
	return 0;
}
