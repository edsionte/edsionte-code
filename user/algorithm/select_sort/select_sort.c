#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

void selectSort(int *r, int length)
{
	int i, j, k, n;

	n = length;

	for ( i = 1; i <= n - 1; i++) {
		k = i;

		for ( j = i + 1; j <= n; j++) 
			if (r[j] < r[k]) 
				k = j;
		if ( k != i) {
			r[0] = r[i];
			r[i] = r[k];
			r[k] = r[0];
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
	printf("Sorting:\n");
	selectSort(r, length);
	printf("After:\n");
	output(r, length);

	return 0;
}
