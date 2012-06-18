#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>

int main()
{
	int fd,n;
	printf("please select a way of creating files:\n");
	printf("1.creat 2.open\n");
	scanf("%d",&n);
	if(n==1)
	{
		if((fd=creat("example_62.c",S_IRWXU))==-1)
		{
			perror("open");
			exit(1);
		}
		else
		{
			printf("create file success\n");
		}
	}
	else if(n==2)
	{
		if((fd=open("example_62.c",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR))==-1)
		{
			perror("open");
			exit(1);
		}
		else
		{
			printf("create file success\n");
		}
	}
	else
	{
		printf("your choice may be wrong\n");
	}
	close(fd);
	return 0;
}
