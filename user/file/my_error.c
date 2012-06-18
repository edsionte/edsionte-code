#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <error.h>

int main()
{
	int fd;
	if((fd=open("example_test.c",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR))==-1)
	{
//		my_err1(errno);
		my_err2("open",__LINE__,errno);
	}
	close(fd);
	return 0;
}
