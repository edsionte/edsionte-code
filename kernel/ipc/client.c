#include <sys/types.h>
#include <linux/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	key_t key;
	int semid,semval;
	union semun semopts;

	if((key=ftok(".",'s'))==-1)
	{
		perror("ftok error\n");
		exit(1);
	}

	if((semid=semget(key,1,IPC_CREAT|0666))==-1)
	{
		perror("semget error\n");
		exit(1);
	}

	while(1)
	{
		if((semval=semctl(semid,0,GETVAL,0))==-1)
		{
			perror("semctl error\n");
			exit(1);
		}
		if(semval>0)
		{
			printf("still %d resources can be used\n",semval);
		}
		else
		{
			printf("No more resources can be used\n");
			break;
		}

		sleep(3);
	}
	exit(0);
}
