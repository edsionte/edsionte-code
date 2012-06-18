#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 80
void my_error(const char*, int, int);

int main(int argc, char **argv)
{
	int sockfd, n;
	char recvline[MAXLINE + 1];
	struct sockaddr_in serv_addr;

	if (argc != 2) {
		printf("Usage:a.out <IPaddress>\n");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		my_error("socket", errno, __LINE__);
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(4555);

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
		my_error("inet_pton", errno, __LINE__);

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) 
		my_error("connect", errno, __LINE__);
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;
		if (fputs(recvline, stdout) == EOF)
			my_error("fputs", errno, __LINE__);
	}

	if (n < 0) 
		printf("Error:read error\n");

	exit(0);
}

void my_error(const char *error_str, int error_num, int line)
{
	printf("Error:%s() with \"%s\" at line %d\n", error_str, strerror(error_num), line);
	exit (1);
}
