/*
 * Author:  edsionte
 * Email:   edsionte@gmail.com
 * Time:    2011/10/26
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main()
{
	int fd;
	char *buf = NULL;
	int i;

	//打开一个文件
	if (-1 == (fd = open("./mapping_file.txt", O_RDWR))) {
		printf("open file error!\n");
		exit(1);
	}

	//将文件映射到进程的一个内存区域
	buf = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (!buf) {
		printf("mmap error!\n");
		exit(1);
	}

	//对映射内存读数据
	for (i = 0; i < 1024; i++)	
		printf("%c", buf[i]);

	//对映射内存写数据
	if (buf[0] == 'H')
		buf[0] = 'h';
	else
		buf[0] = 'H';

	system("cat ./mapping_file.txt");
	sleep(20);
	return 0;
}
