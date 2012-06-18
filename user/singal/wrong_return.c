#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf env;

void handler_sigrtmin15(int signo)
{
	printf("receive SIGRTMIN+15\n");
	longjmp(env,1);
}

void handler_sigint(int signo)
{
	printf("receive SIGINT\n");
	longjmp(env,2);
}

int main()
{
	//set return point
	switch(setjmp(env))
	{
		case 0:
			break;
		case 1:
			printf("return from SIGRTMIN+15\n");
			break;
		case 2:
			printf("return from SIGINT\n");
			break;
		default:
			break;
	}

	signal(SIGRTMIN+15,handler_sigrtmin15);
	signal(SIGINT,handler_sigint);

	while(1)
		;
	return 0;
}
