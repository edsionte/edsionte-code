#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERV_PORT 6666
#define DATA_MAX 1024

void str_cli(FILE *fp, int sockfd)
{
	int maxfdp1, stdineof; //maxfd plus 1;
	fd_set rset;
	char buf[DATA_MAX];
	int n;

	stdineof = 0;
	FD_ZERO(&rset);
	for (;;) {
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		if (fileno(fp) > sockfd)
			maxfdp1 = fileno(fp) + 1;
		else
			maxfdp1 = sockfd + 1;
		if (select(maxfdp1, &rset, NULL, NULL, NULL) == -1) {
			printf("errno at line %d:%s\n", __LINE__, strerror(errno));
			exit(1);
		}

		if (FD_ISSET(sockfd, &rset)) {
			if ((n = read(sockfd, buf, DATA_MAX)) == 0) {
				if (stdineof == 1) 
					return;
				else {
					printf("errno at line %d:server terminated prematurely\n", __LINE__);
					exit(1);
				}
			}
			write(fileno(stdout), buf, n);
		}

		if (FD_ISSET(fileno(fp), &rset)) {
			if ((n = read(fileno(fp), buf, DATA_MAX)) == 0) {
				stdineof == 1;
				shutdown(sockfd, SHUT_WR);
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			write(sockfd, buf, n);
		}
	}
}

int main(int argc, char *argv[])
{
	int sockfd, recvbytes;
	char buf[DATA_MAX];
	struct hostent *host;
	struct sockaddr_in serv_addr;

	if (argc < 2) {
	
		printf("error: please input the server's hostname!\n");
		exit(1);
	}

	if((host = gethostbyname(argv[1])) == NULL) {
		
		printf("gethostbyname error!\n");
		exit(1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	
		printf("socket error!\n");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr = *((struct in_addr *)host->h_addr);

	if(connect(sockfd, (struct sockaddr *)&serv_addr,
				sizeof(struct sockaddr)) == -1) {
		
		printf("connect error!\n");
		exit(1);
	}

	str_cli(stdin, sockfd);

	return 0;
}
