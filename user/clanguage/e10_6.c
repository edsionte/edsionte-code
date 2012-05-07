#include <stdio.h>
#include <stdlib.h>

#define N 10
int main()
{
	int wage[N],temp;
	int i,j,k;

	for(i=0;i<N;i++)
	{
		printf("worker %d:",i+1);
		scanf("%d",&wage[i]);
	}

	//sorting
	for(i=0;i<N;i++)
	{
		k=i;
		for(j=i;j<N;j++)
		{
			if(wage[j]<wage[k]) 
				k=j;
		}
		if(k!=i)
		{
			temp=wage[i];
			wage[i]=wage[k];
			wage[k]=temp;
		}
	}

	for(i=0;i<N;i++)
		printf("%d ",wage[i]);
	printf("\n");
}
