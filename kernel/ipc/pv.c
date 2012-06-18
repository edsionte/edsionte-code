#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
};

int p(int semid,int index)
{
	struct sembuf buf={0,-1,SEM_UNDO};

	if(index<0)
	{
		printf("error:index of the semaphore is invalid\n");
		exit(1);
	}
	buf.sem_num=index;
	if(semop(semid,&buf,1)==-1)
	{
		printf("P operation failed\n");
		exit(1);
	}
	return 0;
}

int v(int semid,int index)
{
	struct sembuf buf={0,+1,SEM_UNDO};

	if(index<0)
	{
		printf("error:index of the semaphore is invalid\n");
		exit(1);
	}
	buf.sem_num=index;
	if(semop(semid,&buf,1)==-1)
	{
		printf("V operation failed\n");
		exit(1);
	}
	return 0;
}

int main(int argc,char** argv)
{
	int proj_id;
	int semid;
	union semun arg;
	pid_t pid;
	key_t key;
	int num;
	int i,j;

	if(argc!=2)
	{
		printf("error:%s num\n",argv[0]);
		return -1;
	}

	num=atoi(argv[1]);

	//create key
	proj_id=2;
	if((key=ftok(".",proj_id))==-1)
	{
		printf("generating IPC key failed\n");
		return -1;
	}

	//create a semaphore set
	if((semid=semget(key,1,IPC_CREAT|0666))==-1)
	{
		printf("creating semaphore set failed\n");
		return -1;
	}

	arg.val=1;
	if(semctl(semid,0,SETVAL,arg)==-1)
	{
		printf("set semval failed\n");
		return -1;
	}

	for(i=0;i<num;i++)
	{
		pid=fork();
		if(pid<0)
		{
			printf("creating new process failed\n");
			return -1;
		}
		else if(pid==0)
		{
			if((semid=semget(key,1,0))==-1)
			{
				printf("geting semid failed in the child process\n");
				return -1;
			}

			p(semid,0);
			printf("===process %d enter the critical section===\n",getpid());
           		sleep(1);
			printf("===process %d is accessing=================\n",getpid());
			sleep(1);
			printf("===process %d leave the critical section===\n",getpid());
			sleep(1);
			v(semid,0);
			sleep(2);
			printf("===process %d is doing another thing=======\n",getpid());
			return 0;
		}
	}

	for(i=0;i<num;i++)
	{
		wait(NULL);
	}

	if(semctl(semid,0,IPC_RMID,0)==-1)
	{
		printf("remove the sem set failed\n");
		return -1;
	}

	return 0;
}
