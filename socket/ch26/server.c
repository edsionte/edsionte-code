#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define SERV_PORT 6666
#define BACKLOG 10
#define DATA_MAX 1024

void my_error(const char *err_string, int error_no, int line_no) 
{
	printf("error at line %d:%s() with %s\n", line_no, err_string, strerror(error_no));
	exit(1);
}

void str_echo(int sockfd)
{
	int n;
	char buf[DATA_MAX];

again:
	while((n = read(sockfd, buf, DATA_MAX)) > 0) 
		if (write(sockfd, buf, n) < 0) {
			printf("write error!\n");
			exit(1);
		}

	if (n < 0 && errno == EINTR) 
		goto again;
	else if (n < 0)
		printf("str_echo error!\n");
}

static void *doit(void *arg)
{
	int connfd;

	connfd = *((int *)arg);
	free(arg);

	if (pthread_detach(pthread_self()) != 0) {
		my_error("pthread_detach", errno, __LINE__);
	}

	str_echo(connfd);
	close(connfd);
	return;
}

int main(int argc, char **argv)
{
	int sockfd, sin_size;
	pthread_t tid;
	struct sockaddr_in my_addr;
	struct sockaddr_in remote_addr;
	int *iptr;

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

	for (;;) {
		sin_size = sizeof(struct sockaddr);
		iptr = malloc(sizeof(int));
		if ((*iptr = accept(sockfd, (struct sockaddr *)&remote_addr, &sin_size)) < 0) 
			my_error("accept", errno, __LINE__);

		if (pthread_create(&tid, NULL, &doit, iptr) != 0)
			my_error("pthread_create", errno, __LINE__);
		printf("parent:%lu, child:%lu\n", pthread_self(), tid);
 	}
			
	return 0;
}

