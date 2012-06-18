#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	int fd[2];
	pid_t pid;
	char rbuf[1024]="this is the read buffer!";
	char wbuf[1024]="this is the write buffer!";
	int readnum;
	int writenum;

	memset(rbuf,0,sizeof(rbuf));
	if(pipe(fd)<0)
	{
		printf("create pipe error\n");
		exit(1);
	}

	if((pid=fork())==0)
	{
		close(fd[0]);
		close(fd[1]);
		sleep(20);
		exit(0);
	}
	else if(pid>0)
	{
		//parent process have read
		sleep(1);
		close(fd[1]);
		readnum=read(fd[0],rbuf,sizeof(rbuf));
		printf("I am parent and the number of reading data is %d\n",readnum);
		exit(0);
	}
	else
	{
		printf("fork error\n");
		exit(1);
	}
}
