#include <shm.h>

void my_sleep(int i)
{
	while(i--)
	{
		printf("reader is sleeping..\n");
		sleep(1);
	}
}

int main()
{
	int semid,shmid;
	char *shmaddr=NULL;

	if((shmid=createshm(".",'m',SHM_SIZE))==-1)
	{
		exit(1);
	}

	if((shmaddr=shmat(shmid,(char*)0,0))==(char *)-1)
	{
		perror("attach shared memory error\n");
		exit(1);
	}

	if((semid=opensem(".",'s'))==-1)
	{
		exit(1);
	}

	while(1)
	{
                waitsem(semid,0);
		p(semid,0);
		printf("reader:");
		printf("%s\n",shmaddr);
		my_sleep(5);
		v(semid,0);
	//	my_sleep(5);
	}
	
}
