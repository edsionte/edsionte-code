#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

#define MAXSIZE 100

int main(int argc, char *argv[])
{
	int sockfd, len;
	int result, count;
	char recvs[MAXSIZE];
	struct sockaddr_in address;
	struct hostent *host;

        if (argc != 3) {
		
		printf("client ip filename\n");
		exit(1);
	}

	host = gethostbyname(argv[1]);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	
		printf("socket create error!\n");
		exit(1);
	}
	
	memset(&address, 0, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	address.sin_addr = *((struct in_addr *)host->h_addr);
	address.sin_port = htons(5000);
	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *)&address, len);

	if (result == -1) {
	
		printf("connect error!\n");
		exit(1);
	}

	FILE *fd = fopen(argv[2], "wb");
	if (fd == NULL) {
		
		printf("fopen error!\n");
		exit(1);
	}

	while (1) {
		
		count = read(sockfd, recvs, MAXSIZE);
		if (count == 0) 
			break;
		fwrite(recvs, 1, count, fd);
	}

	close(sockfd);
	fclose(fd);
	return 0;
}
