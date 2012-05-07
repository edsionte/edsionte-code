#include <stdio.h>
#include <stdlib.h>

#define N 3
typedef struct 
{
	float average;
	float high;
	float low;
}course;

int main()
{
	float score[N][3],average[N]={0};
	course c[3];
	int i;
	float sum[3]={0};

	//get the data
	for(i=0;i<N;i++)
	{
		printf("student %d:\n",i+1);
		printf("course 1:");
		scanf("%f",&score[i][0]);
		printf("course 2:");
		scanf("%f",&score[i][1]);
		printf("course 3:");
		scanf("%f",&score[i][2]);
	}

	//get the average score of each course
	for(i=0;i<N;i++)
	{
		//get the average of each student
		average[i]=score[i][0]+score[i][1]+score[i][2];
		average[i]/=3;

		sum[0]+=score[i][0];
		sum[1]+=score[i][1];
		sum[2]+=score[i][2];
	}

	//get the average of each courses
	for(i=0;i<3;i++)
	{
		c[i].average=sum[i]/N;
                //data initilization
		c[i].high=c[i].low=score[0][i];
	}

	//get the high score and low score of every course
        for(i=0;i<N;i++)
	{
		if(score[i][0]>c[0].high) 
			c[0].high=score[i][0];
		if(score[i][0]<c[0].low)
			c[0].low=score[i][0];
		if(score[i][1]>c[1].high) 
			c[1].high=score[i][1];
		if(score[i][1]<c[1].low)
			c[1].low=score[i][1];
		if(score[i][2]>c[2].high) 
			c[2].high=score[i][2];
		if(score[i][2]<c[2].low)
			c[2].low=score[i][2];

	}

	//print the result
	printf("Students' result:\n");
	for(i=0;i<N;i++)
	{
		printf("%d %3.1f,%3.1f,%3.1f,average=%3.1f\n",i+1,score[i][0],score[i][1],score[i][2],average[i]);
	}
	printf("Courses' result:\n");
	for(i=0;i<3;i++)
	{
		printf("course %d: highest=%3.1f lowest=%3.1f average=%3.1f\n",i+1,c[i].high,c[i].low,c[i].average);
	}

	return 0;




}
