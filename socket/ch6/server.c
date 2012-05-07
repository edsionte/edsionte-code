#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>


#define SERV_PORT 6666
#define DATA_MAX 1024

void my_error(const char *err_string, int errorno, int lineno)
{
	printf("Error at Line %d:%s() with \"%s\"\n", lineno, err_string, strerror(errno));
	exit(1);
}

int main(int argc, char **argv)
{
	int i, maxi, maxfd, listenfd, connfd, sockfd;
	int nready, client[FD_SETSIZE];
	ssize_t n;
	fd_set rset, allset;
	char buf[DATA_MAX];
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		my_error("socket", errno, __LINE__);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) 
		my_error("bind", errno, __LINE__);

	if (listen(listenfd, 100) < 0)
		my_error("listen", errno, __LINE__);

	maxfd = listenfd;
	maxi = -1;
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	for (;;) {
		rset = allset;
		if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0) 
			my_error("select", errno, __LINE__);

		if (FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(cliaddr);
			if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
				my_error("accept", errno, __LINE__);
			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] , 0) {
					client[i] = connfd;
					break;
				}
			if (i == FD_SETSIZE) {
				printf("too many clients!\n");
				exit(1);
			}

			FD_SET(connfd, &allset);
			if (connfd > maxfd)
				maxfd = connfd;
			if (i > maxi)
				maxi = i;

			if (--nready <= 0)
				continue;
		}

		for (i = 0; i <= maxi; i++) {
			if ((sockfd = client[i]) < 0)
				continue;

			if (FD_ISSET(sockfd, &rset)) {
				close(sockfd);
				FD_CLR(sockfd, &allset);
				client[i] = -1;
			} else {
				write(sockfd, buf, n);
			}

			if (--nready <= 0)
				break;
		}
	}
}
