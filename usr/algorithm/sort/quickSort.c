#include <stdio.h>
#include <stdlib.h>

void quickSort(int *, int, int);
int partition(int *, int, int);
void input_data(int *, int);
void output_data(int *, int);

int n = 0;

int main()
{
	int *a = NULL;
	int i;

	printf("Input the length of array:\n");
	scanf("%d", &n);
	a = (int *)malloc(sizeof(int) * n);
	input_data(a, n);
	
	for (i = 0; i < n; i++)
		printf("%d ", i);
	printf("\n");

	printf("Before sorting:\n");
	output_data(a, n);

	printf("\n");
	quickSort(a, 0, n - 1);
	printf("\n");

	printf("after sorting:\n");
	output_data(a, n);

	free(a);

	return 0;
}

/*
 * 以p为基准对a[p, r]进行快速排序
 * q为分界点，最终a[p, q - 1] 小于a[q];a[q + 1, r]大于a[q];
 */
void quickSort(int *a, int p, int r)
{
	if (p < r) {
		int q = partition(a, p, r);
		quickSort(a, p, q - 1);
		quickSort(a, q + 1, r);
	}
}

int partition(int *a, int p, int r)
{
	int x, low, high;

	x = a[p];
	low = p;
	high = r;

	while (low < high) {
		while (low < high && a[high] >= x)
			high--;
		if (low < high) {
			a[low] = a[high];
			a[high] = x;
			low++;
		}
		output_data(a, n);

		while (low < high && a[low] < x)
			low++;
		if (low < high) {
			a[high] = a[low];
			a[low] = x;
			high--;
		}
		output_data(a, n);
	}

	a[low] = x;

	return low;
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


