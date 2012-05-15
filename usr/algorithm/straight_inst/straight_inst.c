#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

void insSort(int *r, int length)
{
	int i, j;

	printf("Sorting:\n");
	for ( i = 2; i <= length; i++) {
		r[0] = r[i];
		j = i - 1;

		while (r[0] < r[j]) {
			r[j + 1] = r[j];
			j--;
		}

		r[j + 1] = r[0];

		output(r, length);
	}
}

int main() 
{
	int r[100] = {0, 48, 62, 35, 77, 55, 14, 35, 98};
	int length = 8;

	printf("Before:\n");
	output(r, length);
	insSort(r, length);
	printf("After:\n");
	output(r, length);

	return 0;
}
