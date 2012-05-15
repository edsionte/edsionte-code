#include <stdio.h>
#include <pthread.h>

void* assisthread(void* arg)
{
	printf("I am helping to do some jobs\n");
	sleep(3);
	pthread_exit(0);
}

int main(void)
{
	pthread_t assisthid;
	int status;

	pthread_create(&assisthid,NULL,assisthread,NULL);
	pthread_join(assisthid,(void*)&status);
	printf("assisthread's exit is caused %d\n",status);

	printf("main thread ends\n");
	return 0;
}
