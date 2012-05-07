#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	umask(0);
	if(creat("example_68_1.c",S_IRWXU|S_IRWXG|S_IRWXO)<0)
	{
		perror("creat");
		exit(1);
	}

	umask(S_IRWXO);
	if(creat("example_68_2.c",S_IRWXU|S_IRWXG|S_IRWXO)<0)
	{
		perror("creat");
		exit(1);
	}
	return 0;

}
