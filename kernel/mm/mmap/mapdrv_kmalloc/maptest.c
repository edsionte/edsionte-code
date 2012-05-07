#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#define LEN (10 * 4096)

int main(void)
{
	int fd, ret = 0;
	char *vadr;
	int i;

	if ((fd = open("/dev/mapdrv_k", O_RDWR)) < 0) {
		perror("open");
		ret = -1;
		goto fail;
	}
	vadr = mmap(0, LEN, PROT_READ, MAP_PRIVATE | MAP_NORESERVE, fd, 0);

	if (vadr == MAP_FAILED) {
		perror("mmap");
		ret = -1;
		goto fail_close;
	}
	
	printf("%s\n", vadr);

	if (-1 == munmap(vadr, LEN))
		ret = -1;
fail_close:
	close(fd);
fail:
	exit(ret);
}
