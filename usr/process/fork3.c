#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

int main()
{
	pid_t pid;
	int i=0;

	pid=fork();
	switch(pid)
	{
		case 0:
			{
				while(1)
				{

					printf("I am child process,my pid is %d,my parent process's pid is %d\n",getpid(),getppid());
					sleep(2);
					i++;
					if(i==3) exit(0);
				}
			}
		case -1:
			{
				printf("process creation failed\n");
				exit(1);
			}
		default:
			{
				printf("I am parent process,my pid is %d\n",getpid());
				exit(0);
			}
	}
	return 0;

}
