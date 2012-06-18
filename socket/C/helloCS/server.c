#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define SERV_PORT 4507
#define BACKLOG 10

int main()
{
	int sockfd, client_fd, sin_size;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	int fd;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	
		printf("socket error!\n");
		exit(1);
	}

	memset(&my_addr, 0, sizeof(struct sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(SERV_PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&my_addr, 
				sizeof(struct sockaddr_in)) == -1) {
	
		printf("bind error!\n");
		exit(1);
	}

	if(listen(sockfd, BACKLOG) == -1) {
	
		printf("listen error!\n");
		exit(1);
	}

	while (1) {
	
		sin_size = sizeof(struct sockaddr_in);
		if ((client_fd = accept(sockfd, (struct sockaddr *)&remote_addr,
						&sin_size)) == -1) {
		
			printf("accept error!\n");
			continue;
		}

		printf("received a connection from:%s\n", inet_ntoa(remote_addr.sin_addr));

		if((fd = fork()) == 0) {
		
			if(send(client_fd, "Hello, I am edsionte!\n", 22, 0) == -1)
				printf("send error!\n");
			close(client_fd);
	//		close(sockfd);
			exit(0);
		} else if (fd > 0) {
		
			close(client_fd);
		} else {
		
			printf("fork error!\n");
		}
	}//while
	return 0;
}

