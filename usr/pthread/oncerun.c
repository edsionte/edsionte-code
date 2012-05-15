#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

pthread_once_t once=PTHREAD_ONCE_INIT;

void run(void)
{
	printf("Function run() is running in thread %lu\n",pthread_self());
}

void* thread1(void* arg)
{
	pthread_t thid=pthread_self();
	printf("current thread's ID is %lu\n",thid);
	pthread_once(&once,run);
	printf("thread1 whose id is %lu ends\n",thid);
}

void* thread2(void* arg)
{
	pthread_t thid=pthread_self();
	printf("current thread's ID is %lu\n",thid);
	pthread_once(&once,run);
	printf("thread2 whose id is %lu ends\n",thid);
}

int main()
{
	pthread_t thid1;
	pthread_t thid2;

	pthread_create(&thid1,NULL,thread1,NULL);
	pthread_create(&thid2,NULL,(void*)thread2,NULL);
	sleep(3);

	printf("main thread exit\n");
	exit(0);
}
