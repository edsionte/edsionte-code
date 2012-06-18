/*
 * Author:  edsionte
 * Email:   edsionte@gmail.com
 * Time:    2011/10/26
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	int srcfd, destfd;
	struct stat statbuf;
	char *src = NULL, *dest = NULL;

	//打开两个文件
	if (-1 == (srcfd = open("./src.txt", O_RDONLY))) {
		printf("open src file error!\n");
		exit(1);
	}

	if (-1 == (destfd = open("./dest.txt", O_RDWR | O_CREAT | O_TRUNC))) {
		printf("open dest file error!\n");
		exit(1);
	}

	//获取原始文件的长度
	if (-1 == fstat(srcfd, &statbuf)) {
		printf("fstat src file error!\n");
		exit(1);
	}

	//设置输出文件的大小
	if (-1 == lseek(destfd, statbuf.st_size - 1, SEEK_SET)) {
		printf("lseek error!\n");
		exit(1);
	}
	if (-1 == write(destfd, "", 1)) {
		printf("write error!\n");
		exit(1);
	}

	if ((src = mmap(0, statbuf.st_size, PROT_READ, 
					MAP_SHARED, srcfd, 0)) == MAP_FAILED) {
		printf("mmaping src file error!\n");
		exit(1);
	}

	if ((dest = mmap(0, statbuf.st_size, PROT_READ | PROT_WRITE, 
					MAP_SHARED, destfd, 0)) == MAP_FAILED) {
		printf("mmaping dest file error!\n");
		exit(1);
	}

	memcpy(dest, src, statbuf.st_size);

	printf("src file:\n");
	system("cat ./src.txt");
	printf("dest file:\n");
	system("cat ./dest.txt");

	close(srcfd);
	close(destfd);

	return 0;
}
