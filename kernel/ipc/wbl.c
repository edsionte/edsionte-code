#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FIFO "fifo_example"

int main(int argc,char** argv)
{
	int fd;
	char wbuf[65536*2];
	int real_wnum;

	memset(wbuf,0,65536*2);
	if((mkfifo(FIFO,O_CREAT|O_EXCL)<0)&&(errno!=EEXIST))
	{
		printf("create fifo failure: fifo exist!\n");
		exit(1);
	}

	fd=open(FIFO,O_WRONLY);

	if(fd==-1)
	{
		if(errno==ENXIO)
			printf("open error: no read process!\n");
		else
			printf("open error!\n");
		exit(1);
	}

	real_wnum=write(fd,wbuf,30000);
	if(real_wnum==-1)
	{
		if(errno==EAGAIN)
			printf("write to fifo error:try later\n");
	}
	else
		printf("real write num is %d\n",real_wnum);

	real_wnum=write(fd,wbuf,60000);
	
	if(real_wnum==-1)
		if(errno==EAGAIN)
			printf("try later\n");

	return 0;

}
