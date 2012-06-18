/*
 * filename: test/test.c
 * Version: 0.1
 * Copyleft (C) Zeliang Li <lizeliang.linux@gmail.com>
 * Date: 2009.8.18
 * */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define GLOBALMEM_SIZE 0x1000
#define GLOBALMEM_MAGIC 0x01
#define MEM_CLEAR _IO(GLOBALMEM_MAGIC, 0)

int main(int argc, char **argv)
{
	int dev_fd;
	char r_buff[GLOBALMEM_SIZE] = {0},w_buff[GLOBALMEM_SIZE] = {0};
	int retval = 0;

	if (argc != 2) {
		printf("Usage: %s [device file]\n", basename(argv[0]));
		exit(0);
	}

	dev_fd = open(argv[1], O_RDWR | O_NONBLOCK);
	if (dev_fd < 0) {
		perror("Open deivce");
		exit(EXIT_FAILURE);
	}
	ioctl(dev_fd, MEM_CLEAR);
	printf("Open device success. Please input you info: ");
	//scanf("%s",w_buff);
	fgets(w_buff, GLOBALMEM_SIZE, stdin);
	retval = write(dev_fd, w_buff, strlen(w_buff));
	//retval = write(dev_fd, w_buff, sizeof(w_buff));
	if (retval < 0) {
		perror("Write device");
		exit(EXIT_FAILURE);
	}
	lseek(dev_fd, 0, SEEK_SET);
	printf("Now, we get the info from the device %s: ", argv[1]);
	retval = read(dev_fd, r_buff, strlen(w_buff));
	//retval = read(dev_fd, r_buff, sizeof(w_buff));
	if (retval < 0) {
		perror("Read device");
		exit(EXIT_FAILURE);
	}
	printf("%s", r_buff);
	close(dev_fd);
	return 0;
}
