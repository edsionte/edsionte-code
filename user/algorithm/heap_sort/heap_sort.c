#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

void adjust_heap(int *r, int k, int m)
{
	int i, j, t;
	int finished;

	r[0] = r[k];
	i = k;
	j = 2 * i;

	finished = 0;

	while (j <= m && !finished) {
		if (j < m && r[j] < r[j + 1])
			j++;
		if (r[0] >= r[j]) 
			finished = 1;
		else {
			r[i] = r[j];
			i = j;
			j = 2 * i;
		}
	}

	r[i] = r[0];
}

void create_heap(int *r, int length)
{
	int i, n;

	n = length;

	for ( i = n / 2; i >= 1; i--)
		adjust_heap(r, i, n);
}

void heapSort(int *r, int length)
{
	int i, n;
	int t;

	create_heap(r, length);

	n = length;

	for ( i = n; i >= 2; i--) {
		t = r[1];
		r[1] = r[i];
		r[i]= t;
		adjust_heap(r, 1, i - 1);
		output(r, length);
	}
}

int main() 
{
	int r[100] = {0, 48, 62, 35, 77, 55, 14, 35, 98};
	int length = 8;

	printf("Before:\n");
	output(r, length);
	printf("Sorint:\n");
	heapSort(r, length);
	printf("After:\n");
	output(r, length);

	return 0;
}
