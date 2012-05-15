/*
 *Author: edsionte
 *Email:  edsionte@gmail.com
 *Time:   2011/03/14
*/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int sec;

	sec = atoi(argv[1]);
	while (sec != 0) {
		sleep(1);
		printf("[New Child %d] I am running and my parent is %d\n", getpid(), getppid());
		sec--;
	}

	return 0;
}
