#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define LISTENQ 1024
#define MAXLINE 80
void my_error(const char*, int, int);

int main(int argc, char **argv) 
{
	int listenfd, connfd;
	struct sockaddr_in serv_addr, client_addr;
	char buf[MAXLINE];
	time_t ticks;
	int addr_size;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		my_error("socket", errno, __LINE__);

	memset(&serv_addr, 0, sizeof(struct sockaddr_in));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(4555);

	if (bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
		my_error("bind", errno, __LINE__);

	if (listen(listenfd, LISTENQ) == -1)
		my_error("listen", errno, __LINE__);

	for (;;) {
		addr_size = sizeof(struct sockaddr);
		if ((connfd = accept(listenfd, (struct sockaddr*)&client_addr, &addr_size)) == -1)
			my_error("accept", errno, __LINE__);
		
		printf("%s has connectd to server..\n", inet_ntoa(client_addr.sin_addr));
		sleep(5);
		ticks = time(NULL);
		snprintf(buf, sizeof(buf), "%.24s\r\n", ctime(&ticks));
		if (write(connfd, buf, strlen(buf)) < 0)
			my_error("write", errno, __LINE__);

		close(connfd);
	}
}

void my_error(const char *error_str, int error_num, int line)
{
	printf("Error:%s() with \"%s\" at line %d\n", error_str, strerror(error_num), line);
	exit (1);
}
