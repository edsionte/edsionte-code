#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define SERV_PORT 6666
#define DATA_MAX 100

static int sockfd;
static FILE *fp;

void my_error(const char *err_string, int error_no, int line_no) 
{
	printf("error at line %d:%s() with %s\n", line_no, err_string, strerror(error_no));
	exit(1);
}

void *copyto(void *arg)
{
	char sendbuf[DATA_MAX];

	printf("fgets_write:%lu\n", pthread_self());
	while(fgets(sendbuf, DATA_MAX, fp) != NULL)
		write(sockfd, sendbuf, strlen(sendbuf));

	shutdown(sockfd, SHUT_WR);

	return;

}

void str_cli(FILE *fp_arg, int sockfd_arg)
{
	char recvbuf[DATA_MAX];
	pthread_t tid;

	sockfd = sockfd_arg;
	fp = fp_arg;

	if (pthread_create(&tid, NULL, copyto, NULL) != 0) {
		my_error("pthread_create", errno, __LINE__);
	}
	printf("read_fputs:%lu\n", pthread_self());

	while (read(sockfd, recvbuf, DATA_MAX) > 0)
		fputs(recvbuf, stdout);
}

int main(int argc, char *argv[])
{
	int recvbytes;
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
