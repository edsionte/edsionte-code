#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char **argv)
{
	char buf[512];

	if(argc<2)
	{
		printf("error:my_cdvc <target path>\n");
		exit(1);
	}
	if(chdir(argv[1])<0)
	{
		perror("chdir");
	}
	if(getcwd(buf,512)<0)
	{
		perror("getcwd");
	}

	printf("%s\n",buf);
	return 0;
}
