#include <stdio.h>
#include <stdlib.h>

void mergeSort(int *, int);
void mergePass(int *, int *, int, int);
void merge(int *, int *, int, int, int);
void input_data(int *, int);
void output_data(int *, int);

int main()
{
	int *a = NULL;
	int i, n;

	printf("Input the length of array:\n");
	scanf("%d", &n);
	a = (int *)malloc(sizeof(int) * n);
	input_data(a, n);
	
	printf("before sorting:\n");
	output_data(a, n);

	mergeSort(a, n);
	
	printf("after sorting:\n");
	output_data(a, n);

	free(a);

	return 0;
}

void mergeSort(int a[], int n)
{
	int *b = NULL;
	int s = 1;
	int count = 1;

	b = (int *)malloc(sizeof(int) * n);
	while (s < n) {
		printf("sort %d:\n", count++);
		mergePass(a, b, s, n);
		s += s;
		printf("sort %d:\n", count++);
		mergePass(b, a, s, n);
		s += s;
	}
	free(b);
}

void mergePass(int x[], int y[], int s, int n)
{
	int i = 0;
	int j;

	while (i < n - 2 * s) {
		merge(x, y, i, i + s - 1, i + 2 * s -1);
		i = i + 2 * s;
	}

	if (i + s < n)
		merge(x, y, i, i + s - 1, n - 1);
	else
		for (j = i; j <= n - 1; j++) 
			y[j] = x[j];

	for (i = 0; i < n; i++)
		printf("%d ", y[i]);
	printf("\n");
}

void merge(int c[], int d[], int l, int m, int r) 
{
	int i, j, k;
	
	i = l;
	j = m + 1;
	k = l;
	while ((i <= m) && (j <= r))
		if (c[i] <= c[j])
			d[k++] = c[i++];
		else 
			d[k++] = c[j++];
	
	int q;
	if (i > m)
		for (q = j; q <= r; q++)
			d[k++] = c[q];
	else
		for (q = i; q <= m; q++)
			d[k++] = c[q];
}

void output_data(int *a, int n)
{
	int i;

	for (i = 0; i < n; i++)
		printf("%d ", a[i]);
	printf("\n");
}

void input_data(int *a, int n)
{
	int i;

	i = 0;
	printf("Now you can input the data:\n");
	while (i < n) {
		printf("a[%d]=", i);
		scanf("%d", &a[i]);
		i++;
	}
}

