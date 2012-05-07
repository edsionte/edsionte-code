#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DATA_NUM 100
int main(void)
{
    int fd;
    char *dev = "sync_globalmem";
    char num_read[DATA_NUM];
    memset(num_read,0,sizeof(num_read));
    printf("hi,I am reader and my pid is %d..\n",getpid());
    if ((fd = open("/dev/mysync",O_RDWR)) != -1) {
        printf("open device %s success!\n",dev);
        printf("read from %s\n",dev);
        read(fd,num_read,sizeof(num_read));
        printf("%s\n",num_read);
        close(fd);
    } else {
        printf("open device %s faild!\n",dev);
    }
    return 0;
}
