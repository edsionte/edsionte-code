#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int fd;
	char buf[32];

	if((fd=open("temp",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))<0)
	{
		perror("open");
	}
	if(unlink("temp")<0)
	{
		perror("unlink");
	}

	printf("A process whose pid is %d is using this file \"temp\"\n",getpid());
	printf("It will be unlink when this process run over\n");
	printf(".....running....\n");
	if(write(fd,"temp",5)<0)
	{
		perror("write");
	}
	if(lseek(fd,0,SEEK_SET)<0)
	{
		perror("lseek");
	}
	if(read(fd,buf,5)<0)
	{
		perror("read");
	}
	printf("the data of file is:%s\n",buf);
	printf("The file \"temp\" is unliked\n");
	return 0;
	
}
