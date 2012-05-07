#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int globvar = 5;

int main()
{
	pid_t pid;
	int var = 1, i, num;

	printf("How to Creat a Process:\n");
	printf("1.fork() 2.vfork()\n");
	scanf("%d", &num);
	
	switch (num) {
		case 1:
			pid = fork();
			break;
		case 2:
			pid = vfork();
			break;
		default:
			printf("the number you inputed just now is illegal\n");
			exit(1);
	}


	switch (pid) {
		case 0:
			i = 3;
			printf("child:i = %d\n", i);
			while (i-- > 0) {
				printf("child process is running..\n");
				globvar++;
				var++;
				sleep(1);
			}
		        printf("child's globvar = %d var = %d\n", globvar, var);
			break;
		case -1:
			printf("create the process failed\n");
			exit(1);
		default:
			i = 3;
			printf("parent:i = %d\n", i);
			while (i-- > 0) {
				printf("parent process is runnig..\n");
				globvar++;
				var++;
				sleep(1);
			}
	                printf("parent's globvar = %d var = %d\n", globvar, var);
			break;
	}
	
	exit(1);
}
