#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define DATA_SIZE 100

int main()
{
	char rdbuf[DATA_SIZE];
	int fd;
	int ret=0;

	if((fd=open("/dev/mysync",O_RDWR))<0)
	{
		printf("reader open failed..\n");
	}
	else
	{
		printf("reader open device success..\n");
		ret=read(fd,rdbuf,sizeof(rdbuf));
		printf("the data is..\n");
		printf("%s\n",rdbuf);
		close(fd);
	}

	return 0;
}
