#include <shm.h>

void my_sleep(int i)
{
	while(i--)
	{
		printf("writer is sleeping..\n");
		sleep(1);
	}
}

int main()
{
	int semid,shmid;
	char *shmaddr=NULL;
	char wbuf[SHM_SIZE];

	if((shmid=createshm(".",'m',SHM_SIZE))==-1)
	{
		exit(1);
	}

	if((shmaddr=shmat(shmid,(char*)0,0))==(char *)-1)
	{
		perror("attach shared memory error\n");
		exit(1);
	}

	if((semid=createsem(".",'s',1,1))==-1)
	{
		exit(1);
	}

	while(1)
	{
                waitsem(semid,0);
		p(semid,0);
		printf("writer:");
		fgets(wbuf,1024,stdin);
		wbuf[strlen(wbuf)-1]='\0';
		strcpy(shmaddr,wbuf);
		my_sleep(5);
		v(semid,0);
	//	my_sleep(5);
	}
	
}
