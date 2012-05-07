#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define GLOBALMEM_SIZE 0x1000
#define MEM_CLEAR 0x1

int main()
{
	int fd;
	char buf[80];
	int ret = 0;
	fd = open("/dev/globalmem",O_RDWR | O_NONBLOCK);
	do {
		ret = read(fd, buf, sizeof(buf));
		printf("%s\n",buf);
	} while (ret == 0);
	getchar();
	ioctl(fd, MEM_CLEAR);
	getchar();
	do {
		printf("Input buf: ");
		scanf("%s",buf);
		ret = write(fd, buf, 12);
	} while (getchar() != '\n');
	getchar();
	do {
		ret = read(fd, buf, sizeof(buf));
		printf("%s\n",buf);
	} while (ret == 0);
	close(fd);
	return 0;
}
