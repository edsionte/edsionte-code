#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc,char* argv[],char** environ)
{
	pid_t pid;
	int stat_val;

	printf("Exec_example!\n");
	pid=fork();
	switch(pid)
	{
		case -1:
			printf("Process creation failed\n");
			exit(1);
		case 0:
			printf("Child process is running\n");
			printf("pid=%d parent's pid=%d ",getpid(),getppid());
			printf("uid=%d gid=%d\n",getuid(),getgid());
			execve("processImage",argv,environ);
			printf("process never go to here!\n");
			exit(0);
		default:
			printf("Parent process is running\n");
	}
	wait(&stat_val);//the parent process is waiting for the child process's running over
	printf("I am parent process! Child process ran over, I will quit.\n");
	exit(0);

}
