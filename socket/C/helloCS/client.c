#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERV_PORT 4507
#define MAX_DATA_NUM 100

int main(int argc, char *argv[])
{
	int sockfd, recvbytes;
	char buf[MAX_DATA_NUM];
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

	if((recvbytes = recv(sockfd, buf, MAX_DATA_NUM, 0)) == -1) {
	
		printf("recv error!\n");
		exit(1);
	}

	buf[recvbytes] = '\0';
	printf("reveived:%s",buf);
	close(sockfd);

	return 0;
}
