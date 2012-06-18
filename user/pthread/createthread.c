#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int *thread(void* arg)
{
	pthread_t newthid;
	newthid = pthread_self();//get the current thread's id
	printf("this a new thread and thread ID is=%lu\n", newthid);
	sleep(1000);
	return NULL;
}

int main()
{
	pthread_t thid;

	printf("main thread,ID is %lu\n", pthread_self());//get the main thread's id
	if (pthread_create(&thid, NULL, (void *)thread, NULL) != 0) {
		printf("thread creation failed\n");
		exit(1);
	}

	printf("my Id is %lu, new thread ID is %lu\n", pthread_self(), thid);
	sleep(1000);
	return 0;
}
