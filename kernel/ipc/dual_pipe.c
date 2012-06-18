#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void child_rw_pipe(int readfd,int writefd)
{
	char message1[100],*message2="The message was read from pipe1.";
	int ret;

	//child process read message from pipe1
	printf("enter child\n");
	if(read(readfd,message1,100)<0)
	{
		printf("read error!");
		exit(1);
	}
	printf("read from pipe1:%s\n",message1);
	//child process write message to pipe2
	ret=write(writefd,message2,strlen(message2)+1);
	printf("child writer write %d Bytes\n",ret);
	/*
	//child process read message from pipe1
	read(readfd,message1,100);
	printf("read from pipe1:%s\n",message1);
	*/
}

void parent_rw_pipe(int readfd,int writefd)
{
	char message2[100],*message1="The message was read from pipe2.";
	int ret;

//	sleep(5);
	printf("enter parent\n");
	/*
	//parent process read message from pipe2
	if(read(readfd,message2,100)<0)
	{
		printf("read error!");
		exit(1);
	}
	printf("read from pipe2:%s\n",message2);
	*/
	//parent process write message to pipe1
	ret=write(writefd,message1,strlen(message1)+1);
	printf("parent writer write %d Bytes\n",ret);
	
	//parent process read message from pipe2
        read(readfd,message2,100);
	printf("read from pipe2:%s\n",message2);
	
}

int main()
{
	int fd1[2],fd2[2];
	pid_t pid;
	int stat_val;

	if(pipe(fd1)!=0)
	{
		printf("pipe1 failed\n");
		exit(1);
	}
	if(pipe(fd2)!=0)
	{
		printf("pipe2 failed\n");
		exit(1);
	}
	pid=fork();
	switch(pid)
	{
		case 0:
			//read from pipe1
			close(fd1[1]);
			//write to pipe2
			close(fd2[0]);
			child_rw_pipe(fd1[0],fd2[1]);
			close(fd1[0]);
			close(fd2[1]);
			exit(0);
		case -1:
			printf("fork error!\n");
			exit(1);
		default:
			//read from pipe2
			close(fd2[1]);
			//write to pipe1
			close(fd1[0]);
			parent_rw_pipe(fd2[0],fd1[1]);
			wait(&stat_val);
			close(fd2[0]);
			close(fd1[1]);
			exit(0);
	}

	return 0;
}
