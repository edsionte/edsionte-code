#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#define FIFO_NAME "myfifo"
#define BUF_SIZE 1024

int main()
{
	int fd;
	char buf[BUF_SIZE];

	umask(0);
	if((fd=open(FIFO_NAME,O_RDONLY))<0)
	{
		printf("Open error\n");
		exit(1);
	}
	printf("I will read data from FIFO..\n");
	read(fd,buf,BUF_SIZE);	
	printf("Read content:%s\n",buf);
	close(fd);
	exit(0);
}

