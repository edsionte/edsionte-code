/*
 * filename: test/test.c
 * Version: 0.2
 * Copyleft (C) Zeliang Li <lizeliang.linux@gmail.com>
 * Date: 2009.8.19
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
	char devfname[20] = {0}, filename[20] = {0};
	char r_buff[GLOBALMEM_SIZE] = {0},w_buff[GLOBALMEM_SIZE] = {0};
	int dev_fd, file_fd;
	int retval = 0;

	if (argc < 2 || argc > 3) {
		printf("Usage: %s [device file] [file name]\n", basename(argv[0]));
		exit(0);
	}
	if (argc > 1) {
		strcpy(devfname, argv[1]);
	}
	if (argc > 2) {
		strcpy(filename, argv[2]);
	}
	
	dev_fd = open(devfname, O_RDWR | O_NONBLOCK);
	if (dev_fd < 0) {
		perror("Open deivce");
		exit(EXIT_FAILURE);
	}
	ioctl(dev_fd, MEM_CLEAR);
	if (argc == 2) {
		printf("Open device success. Please input you info: ");
		//scanf("%s",w_buff);
		fgets(w_buff, GLOBALMEM_SIZE, stdin);
	} else {
		file_fd = open(filename, O_RDONLY);
		if (file_fd < 0) {
			perror("open file");
			exit(EXIT_FAILURE);
		}
		retval = read(file_fd, w_buff, GLOBALMEM_SIZE);
		if (retval < 0) {
			perror("Read file");
			exit(EXIT_FAILURE);
		}
		close(file_fd);
	}
	do {
		retval = write(dev_fd, w_buff, strlen(w_buff));
		//retval = write(dev_fd, w_buff, sizeof(w_buff));
		if (retval < 0) {
			perror("Write device");
			exit(EXIT_FAILURE);
		}
	} while (retval == 0);
	lseek(dev_fd, 0, SEEK_SET);
	//ioctl(dev_fd, MEM_CLEAR);
	printf("Now, we get the info from the device %s: \n", argv[1]);
	do {
		retval = read(dev_fd, r_buff, strlen(w_buff));
		//retval = read(dev_fd, r_buff, sizeof(w_buff));
		if (retval < 0) {
			perror("Read device");
			exit(EXIT_FAILURE);
		}
		printf("%s", r_buff);
	} while (retval == 0);
	close(dev_fd);
	return 0;
}
