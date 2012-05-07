#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXSIZE 100

int main(int argc, char *argv[]) 
{
	int server_sockfd, client_sockfd;
	int server_len, client_len;
	char ch[MAXSIZE];
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	if (argc != 2) {
	
		printf("server: file name\n");
		exit(1);
	}

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sockfd < 0) {
	
		printf("socket error!\n");
		exit(1);
	}

	memset(&server_address, 0, sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(5000);
	server_len = sizeof(server_address);

	int opt = 1;

	setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0) {
	
		printf("bind error!\n");
		exit(1);
	}

	if(listen(server_sockfd, 5) == -1) {
		
		printf("listen error!\n");
		exit(1);
	}

	client_len = sizeof(client_address);
	client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
	if (client_sockfd == -1) {
	
		printf("accept error!\n");
		exit(1);
	}

	FILE *fd = fopen(argv[1], "rb");
	if (fd == NULL) {
	
		printf("file open error!\n");
		exit(2);
	}

	while (!feof(fd)) {
	
		int len = fread(ch, 1, MAXSIZE, fd);
		write(client_sockfd, ch, len);
	}

	close(client_sockfd);
	fclose(fd);
	return 0;
}
