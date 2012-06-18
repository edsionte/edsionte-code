#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

//read pipe
void read_from_pipe(int fd)
{
	char message[100];

	read(fd,message,100);
	printf("read form pipe:%s\n",message);
}

//write pipe
void write_to_pipe(int fd)
{
	char *message="hello,pipe!";
	write(fd,message,strlen(message)+1);
}

int main()
{
	int fd[2];
	pid_t pid;
	int stat_val;

	if(pipe(fd)!=0)
	{
		printf("create pipe failed!\n");
		exit(1);
	}

	switch(pid=fork())
	{
		case -1:
			printf("fork error!\n");
			exit(1);
		default:
			//child process will read message from pipe
			close(fd[1]);
			read_from_pipe(fd[0]);
			printf("child over\n");
			wait(&stat_val);
			exit(0);
		case 0:
			//parent process will write message to pipe
			close(fd[0]);
			write_to_pipe(fd[1]);
			printf("parent over!\n");
			exit(0);
	}

	return 0;
}
