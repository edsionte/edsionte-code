#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>

int main()
{
	pid_t child_pid;
	int k;
	char *msg = NULL;

	child_pid = fork();

	if(child_pid > 0)
	{
		printf("[parent %d]:I have a child whose pid is %d\n",getpid(),child_pid);
		msg = "Parent process is running!\n";
		k = 4;
	}
	else if (0 == child_pid)
        {

		printf("I am child process and my pid is %d\n",getpid());
		msg = "Child process is running!\n";
		k = 2;

	} else {
		printf("fork() error\n");
	}

	while (k) {
		k--;
		sleep(1);
		printf("%s", msg);
	}

	return 0;
}


