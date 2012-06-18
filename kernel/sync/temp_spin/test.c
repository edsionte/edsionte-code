#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#define DATA_SIZE 100

int main()
{
	char rdbuf[DATA_SIZE];
	char wrbuf[DATA_SIZE];
	char *dev="edsionte_cdev";
	int fd;
	int ret=0;

	memset(rdbuf,0,sizeof(rdbuf));
	memset(wrbuf,0,sizeof(wrbuf));

	if((fd=open("/dev/myspin",O_RDWR))<0)
	{
		printf("open %s failed..\n",dev);
		return -1;
	}
	else
	{
		printf("input some words..\n");
		scanf("%s",wrbuf);
		ret=write(fd,wrbuf,sizeof(wrbuf));
		ret=read(fd,rdbuf,sizeof(rdbuf));
		printf("the data is %s\n",rdbuf);
	}

	return 0;
}

