#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char**argv)
{
	int n;
	char buf[1024];

	while(1)
	{
		if(n=read(0,buf,1024)>0)
		{
			buf[n]='\0';
			printf("ctrlprocess receive:%s\n",buf);
			if(strcmp(buf,"exit")==0)
				exit(0);
			if(strcmp(buf,"getpid")==0)
			{
				printf("My pid:%d\n",getpid());
				sleep(3);
				exit(0);
			}
		}
	}
}
