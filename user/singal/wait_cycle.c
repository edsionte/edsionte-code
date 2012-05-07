#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#define UNHAPPEN 0
#define HAPPEN   1

int flag_happen;

void handler_sigint(int signo)
{
	flag_happen=HAPPEN;
}

int main()
{
	if(signal(SIGINT,handler_sigint)==SIG_ERR)
	{
		perror("signal");
		exit(1);
	}

	while(1)
	{
		if(flag_happen==HAPPEN)
		{
			printf("event happened\n");
			break;
		}
	}
	return 0;
}
