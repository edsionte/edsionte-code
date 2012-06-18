#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//PIPE_BUF = 4096  65536
int main()
{
	int fd[2];
	int ret,request;
	char rbuf[50000];
	char wbuf[65000];
	char wbuf2[100000];
	char *msg="hello,pipe!",*msg2="hello,I come here ,too!";
	pid_t pid;
	int stat_val;

	if(pipe(fd)<0)
	{
		printf("pipe error\n");
		exit(1);
	}

	if((pid=fork())==0)
	{
		//child process write
		close(fd[0]);
		ret=write(fd[1],wbuf,65000);
		printf("I am writer and I write %d Byte data\n",ret);
		ret=write(fd[1],wbuf2,100000);
		printf("I am writer and I write %d Byte data\n",ret);
		printf("writer has over\n");
		close(fd[1]);
		exit(0);
	}
	else if(pid>0)
	{
		//parent process read
//		sleep(3);
		close(fd[1]);
		while(1)
		{
			sleep(1);
			ret=read(fd[0],rbuf,10000);
			printf("I am reader and the num of reading data is %d\n",ret);
		}
		close(fd[0]);
		exit(0);
	}
}
