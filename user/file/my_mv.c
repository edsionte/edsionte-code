#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc,char ** argv)
{
	//check the number of argument
	if(argc<3)
	{
		printf("error: my_mv <old file> <new file>\n");
		exit(1);
	}

	if(rename(argv[1],argv[2])<0)
	{
		perror("rename");
		exit(1);
	}
	return 0;
}
