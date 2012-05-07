#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc,char** argv,char** environ)
{
	int fd[2];
	pid_t pid;
	int stat_val;

	if(argc<2)
	{
		printf("wrong parameters\n");
		exit(0);
	}
	if(pipe(fd))
	{
		perror("pipe failed");
		exit(1);
	}

	pid=fork();
	switch(pid)
	{
		case -1:
			perror("fork failed");
			exit(1);
		case 0:
			dup2(fd[0],0);
			execve("ctrlprocess",(void*)argv,environ);
			exit(0);
		default:
			close(fd[0]);
			write(fd[1],argv[1],strlen(argv[1]));
			break;

	}
	wait(&stat_val);
	exit(0);
}
