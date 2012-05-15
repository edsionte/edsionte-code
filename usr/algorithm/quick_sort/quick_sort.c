#include <stdio.h>
#define MAX 100

void output(int *r, int length)
{
	int i = 1;

	for (; i <= length; i++) 
		printf("%d ", r[i]);
	printf("\n");
}

int quickPass(int *r, int left, int right) 
{
	int low, high;

	low = left;
	high = right;
	r[0] = r[low];

	while (low < high) {
		while (low < high && r[high] >= r[0])//只要high比r[0]小则不断往前倒退
			high--;
		if (low < high) {
			r[low] = r[high];
			low++;
		}

		while (low < high && r[low] <= r[0])
			low++;
		if (low < high) {
			r[high] = r[low];
			high--;
		}
	}

	r[low] = r[0];
	return low;
}

void quickSort(int *r, int low, int high, int length)
{
	int pos;


	if (low < high) {
		pos = quickPass(r, low, high);
		quickSort(r, low, pos - 1, length);
		quickSort(r, pos + 1, high, length);
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
	quickSort(r, 1, length, length);
	printf("After:\n");
	output(r, length);

	return 0;
}
