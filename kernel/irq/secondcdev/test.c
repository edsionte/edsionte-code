#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
int main()
{
	int fd;
	int counter=0;
	int old_counter=0;

	//open the /dev/secondcdev device file
	if((fd=open("/dev/secondcdev",O_RDONLY))<0)
	{
		printf("Device open failed\n");
		return -1;
	}

	while(1)
	{
		read(fd,&counter,sizeof(unsigned int));//read the current second number
		if(counter!=old_counter)
		{
			printf("seconds after open /dev/second:%d\n",counter);
			old_counter=counter;
		}
	}

	return 0;
}
