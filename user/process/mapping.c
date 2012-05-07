#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>

int main()
{
	int i,fd;
	char *buf = NULL;

	fd = open("./test.c", O_RDONLY);
	if(fd < 0)
	{
		printf("open error\n");
		return -1;
	}

	buf = mmap(NULL, 12, PROT_READ, MAP_PRIVATE ,fd, 0);

	for(i = 0;i < 12;i++)
	{
		printf("%c",buf[i]);
	}
	printf("\n");
	
	return 0;
}
