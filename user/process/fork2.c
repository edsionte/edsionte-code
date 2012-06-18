#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

int main()
{
	pid_t pid;
	char* msg=NULL;
	int k;

	static int n=1;
	pid=fork();
	switch(pid)
	{
		case 0:
			{
				msg="child process is running\n";
				k=3;
				n++;
				break;
			}
		case -1:
			{
				msg="process creation failed\n";
				break;
			}
		default:
			{
				msg="parent process is running\n";
				k=5;
				n++;
				break;
			}
	}
	while(k>0)
	{
		printf("%s and n=%d\n",msg,n);
		sleep(1);
		k--;
	}
	return 0;
//	exit(0);
}
