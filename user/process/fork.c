#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	pid_t pid;
	int i = 0;

	printf("I am a process %d..\n", getpid());

	pid = fork();
	if (-1 == pid) {
		printf("fork() error!\n");
		return -1;
	} else if (0 == pid) {
		for(;i < 10; i++) {
			printf("[child]:I am process %d..\n", getpid());
			sleep(1);
		}
		sleep(20);
	} else {
		for(;i < 10; i++) {
			printf("[parent]:I am process %d..\n", getpid());
			sleep(1);
		}
		sleep(1000);
	}

	return 0;
}
