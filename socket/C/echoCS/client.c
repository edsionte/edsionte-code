#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERV_PORT 6666
#define DATA_MAX 100

void str_cli(FILE *fp, int sockfd)
{
	char sendline[DATA_MAX], recvline[DATA_MAX];

	while(fgets(sendline, DATA_MAX, fp) != NULL) {
		if (write(sockfd, sendline, strlen(sendline)) < 0) {
			printf("write error!\n");
			exit(1);
		}

		if (read(sockfd, recvline, DATA_MAX) == 0) {
			printf("read error!\n");
			exit(1);
		}

		fputs(recvline, stdout);
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
