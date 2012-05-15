#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_NUM 3
unsigned long long global_sum,count[MAX_NUM];

void* thread_work(void*);

int main()
{
	int i;
	char ch;
	unsigned long long sum = 0;
	int idx[MAX_NUM] = {0,1,2};

	pthread_t pthread_pid[MAX_NUM];

	for(i = 0;i < MAX_NUM;i++){
		if(pthread_create(&pthread_pid[i],NULL,thread_work,&idx[i])!=0)
		{
			perror("pthread_create");
			return -1;
		}
		else
		{
			printf("[thread %d]:tid is %lu\n",i,pthread_pid[i]);
		}
	}

	do{
		sum = 0;
		for(i = 0; i < MAX_NUM; i++)
		{
			sum += count[i];
			printf("count[%d] = %llu ",i,count[i]);
		}

		printf("sum = %llu,global_sum = %llu\n",sum,global_sum);
	}while((ch=getchar()) != 'q');

	printf("return form main..\n");
	return 0;
}

void* thread_work(void* p)
{
	int i = *(int*)p;
	for(;;){
		count[i]++;
		global_sum++;
	}
	printf("return from thread work..\n");

}
