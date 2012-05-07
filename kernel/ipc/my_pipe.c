#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv)
{
	int fd[2];
	int pid;

	//check if the argument is legal

	if(argc!=3)
	{
		printf("my_pipe:%s <command1> <command2>\n",argv[0]);
		exit(1);
	}

	if(pipe(fd)==-1)
	{
		printf("my_pipe:can't create a pipe\n");
		exit(1);
	}

	pid=fork();
	if(pid==0)
	{
		close(fd[0]);
		dup2(fd[1],STDOUT_FILENO);
		close(fd[1]);

		if(execlp(argv[1],argv[1],NULL)==0)
		{
			printf("my_pipe:can't execute the first command in the child process\n");
			exit(1);
		}
	}
	else if(pid>0)
	{
		close(fd[1]);
		dup2(fd[0],STDIN_FILENO);
		close(fd[0]);

		if(execlp(argv[2],argv[2],NULL)==0)
		{
			printf("my_pipe:can't execute the second command in the parent process\n");
			exit(1);
		}
	}
	else
	{
		printf("my_pipe:can't create a new process\n");
		exit(1);
	}

	return 0;
}
