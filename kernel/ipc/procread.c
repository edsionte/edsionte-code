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
	if((fd=open(FIFO_NAME,O_RDWR))<0)
	{
		printf("Open error\n");
		exit(1);
	}
	int n=20;
	while(n--)
	{
		read(fd,buf,1);
		buf[strlen(buf)]='\0';
		printf("Read content:%s\n",buf);
		sleep(2);
	}
	close(fd);
	unlink(FIFO_NAME);
	exit(0);
}

