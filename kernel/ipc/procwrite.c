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
	char buf[BUF_SIZE]="hello procwrite, I come from process named procread!";

	umask(0);

	if(mkfifo(FIFO_NAME,S_IFIFO|0666)==-1)
	{
		perror("mkfifo error!\n");
		exit(1);
	}


	if((fd=open(FIFO_NAME,O_RDWR))==-1)
	{
		perror("fopen error!\n");
		exit(1);
	}

	printf("Now the writer will write data to FIFO..\n");
	write(fd,buf,strlen(buf)+1);
	printf("writer will leave!\n");
	sleep(20);
	strcpy(buf,"The write write another data now");
	write(fd,buf,strlen(buf)+1);
	close(fd);
	exit(0);
}

