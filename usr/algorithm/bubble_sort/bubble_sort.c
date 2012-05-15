#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

void bubbleSort(int *r, int length)
{
	int n, change;
	int i, j, k;

	n = length;
	change = 1;

	for ( i = 1; i <= n - 1 && change; i++) {
		change = 0;
		for ( j = 1; j <= n - i; j++)
			if (r[j] > r[j + 1]) {
				r[0] = r[j];
				r[j] = r[j + 1];
				r[j + 1] = r[0];
				change = 1;
			}
		output(r, length);
	}
}

int main() 
{
	int r[100] = {0, 48, 62, 35, 77, 55, 14, 35, 98};
	int length = 8;

	printf("Before:\n");
	output(r, length);
	printf("Sorting:");
	bubbleSort(r, length);
	printf("After:\n");
	output(r, length);

	return 0;
}
