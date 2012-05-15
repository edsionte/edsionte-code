#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

void shellInsert(int *r, int length, int d)
{
	int i, j;
	int k;
	printf("Sorting:\n");
	for (i = 1 + d; i <= length; i++) {
		if (r[i] < r[i - d]) {
			r[0] = r[i];
			
			for (j = i - d; j > 0 && r[0] < r[j]; j -= d) {
				r[j + d] = r[j];
			}
			r[j + d] = r[0];
		}
	}
	output(r, length);
}

void shellSort(int *r, int length, int *delta, int n)
{
	int i;
	for ( i = 0; i < n; i++) {
//		printf("%d\n", i);
		shellInsert(r, length, delta[i]);
	}
}

int main() 
{
	int r[MAX] = {0, 48, 62, 35, 77, 55, 14, 35, 98};
	int length = 8;
	int delta[MAX] = {4, 3, 2, 1};
	int n = 4;

	printf("Before:\n");
	output(r, length);
	shellSort(r, length, delta, n);
	printf("After:\n");
	output(r, length);

	return 0;
}
