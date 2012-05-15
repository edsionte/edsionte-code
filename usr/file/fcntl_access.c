#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
	int ret;
	int access_mode;
	int fd;

	//create the file
	if((fd=open("example_64.c",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
	{
		perror("open");
	}
	//set file opening way
	if((ret=fcntl(fd,F_SETFL,O_APPEND))<0)
	{
		perror("fcntl");
	}

	//get file opening way
	if((ret=fcntl(fd,F_GETFL,0))<0)
	{
		perror(fcntl);
	}

	access_mode=ret&O_ACCMODE;
	if(access_mode==O_RDONLY){
		printf("example_64 access mode:read only");
	}
	else if(access_mode==O_WRONLY)
	{
		printf("example_64 access mode:write only");
	}
	else if(access_mode=O_RDWR)
	{
		printf("example_64 acess mode:read+write");
	}

	if(ret&O_APPEND)
	{
		printf(",append");
	}
	if(ret&O_NONBLOCK)
	{
		printf(",nonblock");
	}
	if(ret&O_SYNC)
	{
		printf(",sync");
	}
	printf("\n");

	close(fd);
	return 0;
}

