#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DATA_NUM 100
int main(void)
{
    int fd;
    char *dev = "sync_globalmen";
    char num_write[DATA_NUM];
    memset(num_write,0,sizeof(num_write));
    printf("hi,I am writer and my pid is %d..\n",getpid());
    if ((fd = open("/dev/mysync",O_RDWR)) != -1){
        printf("open device %s success!\n",dev);
        printf("write to %s some data\n",dev);
        scanf ("%s",num_write);
        write(fd,num_write,sizeof(num_write));
        close(fd);
    } else {
        printf("open device %s faild!\n",dev);
    }
    return 0;
}
