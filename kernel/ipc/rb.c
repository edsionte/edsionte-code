#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FIFO "fifo_example"

int main(int argc,char ** argv)
{
	char rbuf[65536*2];
	int fd;
	int rnum;
	int ret;

	rnum=atoi(argv[1]);
	printf("the number of requred real read bytes is %d\n",rnum);
	memset(rbuf,0,sizeof(rbuf));

	fd=open(FIFO,O_RDONLY);
	if(fd==-1)
	{
		if(errno==ENXIO)
		{
			printf("no write process\n");
		}
		else
		{
			printf("open error\n");
			exit(1);
		}
	}

	while(1)
	{
		memset(rbuf,0,sizeof(rbuf));
		ret=read(fd,rbuf,rnum);
		if(ret==-1)
			if(errno=EAGAIN)
			{
				printf("no data avlaibel\n");
			
			}
		printf("real read bytes:%d\n",ret);
		sleep(1);
	}
	pause();
	unlink(FIFO);
	return 0;
}

