#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //read
#include <string.h> //strerror()
#include <linux/types.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "minilzo.h"

#define MAX_LEN 1024

#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

//the wkmem is used for compressed
static HEAP_ALLOC(wkmem, LZO1X_1_MEM_COMPRESS);

//error handler
void printerror(int eline, int eno, const char *estring)
{
	printf("Error at line %d:  %s:%s\n", eline, estring, strerror(eno));
	exit(1);
}

int main()
{
	int cfd;
	char *srcbuffer = NULL, *destbuffer = NULL;
	lzo_uint inlen = MAX_LEN;
	lzo_uint outlen = 0;
	
	//init the lzo lib
	if (lzo_init() != LZO_E_OK) {
		printf("lzo_init error.\n");
		return -1;
	}
	

	if ((cfd = open("./minilzo.c", O_RDONLY)) < 0) {
		printerror(__LINE__, errno, "open");
	}

	//alloc the src buffer and dest buffer
	if ((srcbuffer = malloc(MAX_LEN)) < 0) {
		printerror(__LINE__, errno, "malloc");
	}
	if ((destbuffer = malloc(MAX_LEN)) < 0) {
		printerror(__LINE__, errno, "malloc");
	}
	memset(srcbuffer, 0, MAX_LEN);
	memset(destbuffer, 0, MAX_LEN);

	if (read(cfd, srcbuffer, MAX_LEN) < 0) {
		printerror(__LINE__, errno, "read");
	}

	//compress from @srcbuffer to @destbuffer
	int ret;
	if ((ret = lzo1x_1_compress(srcbuffer, inlen, destbuffer, &outlen, wkmem)) != LZO_E_OK) {
		printf("compress error:%d.\n", ret);
		return -1;
	} else {
		printf("compress %lu bytes into %lu bytes.\n", (unsigned long)inlen, (unsigned long)outlen);
	}

	//the compressed data was writen into a new file
	int newfd;
	if ((newfd = open("./cfile", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH)) < 0) {
		printerror(__LINE__, errno, "open");
	}

	if (write(newfd, destbuffer, outlen) < 0) {
		printerror(__LINE__, errno, "write");
	}

	close(cfd);
	close(newfd);

	printf("compress success!\n");
	return 0;
}
