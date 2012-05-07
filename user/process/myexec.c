/*
 *Author: edsionte
 *Email:  edsionte@gmail.com
 *Time:   2011/03/14
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int pid;
	int val = 0;

	printf("argc = %d\n",argc);
	pid = fork();
	if (pid == 0) {
		//child proces;
		printf("[Child %d] I will be a new process!\n", getpid());
		if (execl("./sleeping","first arg", "20", NULL) == -1) {
			printf("execl error!\n");
			exit(1);
		}
		printf("[Child %d] I never go here!\n", getpid());
	} else if (pid > 0) {
		printf("[Parent %d] I am running!\n", getpid());
		/*
		wait(&val);
		printf("[Parent %d] All my child were leaving..\n", getpid());
		*/
	} else {
		printf("fork error!\n");
		exit(1);
	}

	printf("[process %d] I am leaving..\n", getpid());
	return 0;
}
