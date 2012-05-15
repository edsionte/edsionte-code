#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

/*the number of created child processes*/
#define MAX_CHILD_NUMBER 10
#define SLEEP_INTERVAL 2

int child_idx;
void do_something();

main(int argc,char* argv[])
{
	int child_proc_number;
	int i,ch;
	pid_t child_pid;
	pid_t pid[10]={0};/*save every child process's pid*/

	if(argc<2)
	{
		printf("[Usage]:./filename number\n");
		return -1;
	}
	else
	{
		child_proc_number = atoi(argv[1]);
		child_proc_number = (child_proc_number > MAX_CHILD_NUMBER)?MAX_CHILD_NUMBER:child_proc_number;
	}

	for(i=0;i<child_proc_number;i++)
	{
		child_pid=fork();
		if(child_pid>0)
		{
			printf("[parent %d]:I have a child whose pid is %d\n",getpid(),child_pid);
			pid[i]=child_pid;
		}
		else if(0==child_pid)
		{
			child_idx=i;
			do_something();
		}
		else
		{
			printf("fork() error\n");
		}
	}

	/*kill the process that you choose*/
	while((ch = getchar()) != 'q')
	{
		if(isdigit(ch))
		{
			kill(pid[ch-'0'],SIGTERM);
			printf("[parent %d]:I kill the child process %d\n",getpid(),pid[ch-'0']);
		}
		
	}

	exit(0);
//	kill(0,SIGTERM);
	printf("all child processes were been killed\n");

	return 0;
}

void do_something()
{
	for(;;)
	{
		printf("[child %d]:my pid is %d\n",child_idx,getpid());
		sleep(1);
	}
}


