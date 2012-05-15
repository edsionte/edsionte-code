#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t number_mutex;
int             globalnumber;

void write_globalnumber()
{
	pthread_mutex_lock(&number_mutex);
	globalnumber++;
	pthread_mutex_unlock(&number_mutex);
}

int read_globalnumber()
{
	int temp;

	pthread_mutex_lock(&number_mutex);
	temp=globalnumber;
	pthread_mutex_unlock(&number_mutex);
	return temp;
}

int main()
{
	printf("init_return=%d\n",pthread_mutex_init(&number_mutex,NULL));
	write_globalnumber();
	printf("the temp=%d\n",read_globalnumber());
	printf("trylock_return=%d\n",pthread_mutex_trylock(&number_mutex));
	printf("destroy_return=%d\n",pthread_mutex_destroy(&number_mutex));
	return 0;
}
