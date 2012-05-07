#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define DATA_SIZE 100

int main()
{
	char wrbuf[DATA_SIZE];
	int fd,ret=0;

	if((fd=open("/dev/mysync",O_RDWR))<0)
	{
		printf("wirter open failed..\n");
	}
	else
	{
		printf("writer open success..\n");
		printf("input words..\n");
		scanf("%s",wrbuf);
		ret=write(fd,wrbuf,sizeof(wrbuf));
		printf("writed data..\n");
		close(fd);
	}

	return 0;
}
