#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[],char **environ)
{
	int i;

	printf("I am a process image!\n");
	printf("pid=%d parent's pid=%d uid=%d gid=%d\n",getpid(),getppid(),getuid(),getgid());
	for(i=0;i<argc;i++)
	{
		printf("argv[%d]:%s\n",i,argv[i]);
	}
 
	printf("I am child process and I am quiting!\n");
	return 0;
}
