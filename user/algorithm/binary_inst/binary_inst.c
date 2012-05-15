#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

void binSort(int *r, int length)
{
	int i, j;
	int low ,high, mid;

	printf("Sorting:\n");
	for ( i = 2; i <= length; i++) {
		r[0] = r[i];
		low = 1;
		high = i - 1;

		while (low <= high) {
			mid = (low + high) / 2;
			if (r[0] < r[mid])
				high = mid - 1;
			else
				low = mid + 1;
		}//循环退出的条件必然是low>high,即未知关系为high,low,i

		for (j = i - 1; j >= low; j--) 
			r[j + 1] = r[j];
		r[high + 1] = r[0];
		output(r, length);

	}
}

int main() 
{
	int r[100] = {0, 48, 62, 35, 77, 55, 14, 35, 98};
	int length = 8;

	printf("Before:\n");
	output(r, length);
	binSort(r, length);
	printf("After:\n");
	output(r, length);

	return 0;
}
