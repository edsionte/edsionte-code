#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//PIPE_BUF = 4096
int main()
{
	int fd[2];
	int ret,request;
	char rbuf[4096];
	char wbuf[5000];
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
		ret=write(fd[1],msg,strlen(msg)+1);
		printf("I am writer and I write %d Byte data\n",ret);
          	sleep(5);
		ret=write(fd[1],msg2,strlen(msg2)+1);
		printf("I am writer and I write %d Byte data\n",ret);
		sleep(5);
		close(fd[1]);
		printf("child process be closed the write\n");
		exit(0);
	}
	else if(pid>0)
	{
		//parent process read
		close(fd[1]);
		request=5000;
		ret=read(fd[0],rbuf,request);
		printf("when reader request %d Byte,the actual return Bytes is %d\n",request,ret);
		request=10;
		ret=read(fd[0],rbuf,request);
		printf("when reader requesr %d Byte,the actual return Bytes is %d\n",request,ret);
		request=100;
		ret=read(fd[0],rbuf,request);
		printf("when reader request %d Byte,the actual return Bytes is %d\n",request,ret);
		request=1;
		ret=read(fd[0],rbuf,request);
		printf("when reader request %d Byte,the actual return Bytes is %d\n",request,ret);
		printf("I find child process's writer be closed and I will be over\n");
		close(fd[0]);
		exit(0);
	}
}
