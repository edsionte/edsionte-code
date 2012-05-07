#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
 
 
int main(void)
{
	int testdev;
	int i,rf=0;
	char rbuf[1024];
        char wbuf[1024]="Hi,I am edsionte!";

	memset(rbuf, 0, sizeof(rbuf));

	printf("Input any words you like..\n");
	gets(wbuf);
	testdev = open("/dev/globalmem",O_WRONLY);
	if ( testdev == -1 )
	{
		perror("open\n");
		exit(0);
	}
	rf=write(testdev,wbuf,sizeof(wbuf));
	close(testdev);
	testdev = open("/dev/globalmem",O_RDONLY);
	if ( testdev == -1 )
	{
		perror("open\n");
		exit(0);
	}
	rf=read(testdev,rbuf,1024);
	if(rf<0)
		perror("read error\n");
	printf("\n\nNow the following string is what you inputed..\n\n%s\n",rbuf);
	close(testdev);
	return 0;
}
