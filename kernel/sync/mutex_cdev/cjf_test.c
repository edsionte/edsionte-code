#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define DATA_NUM 20
int main(void)
{
    int fd;
    char *dev = "mutex_globalmem";
    char num_write[DATA_NUM],num_read[DATA_NUM];
    memset(num_write,0,sizeof(num_write));
    memset(num_read,0,sizeof(num_read));
    if ((fd = open("/dev/mutex_globalmem",O_RDWR)) != -1){
        printf("open device %s success!\n",dev);
        printf("write to %s some data\n",dev);
        scanf ("%s",num_write);
        write(fd,num_write,sizeof(num_write));
        printf("read from %s\n",dev);
        read(fd,num_read,sizeof(num_read));
        printf("%s\n",num_read);
        close(fd);
    } else {
        printf("open device %s faild!\n",dev);
    }
    return 0;
}
