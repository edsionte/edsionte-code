#include "stdio.h"
#include "stdlib.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"
#include "edsionte_error.h"
#include "errno.h"
#include "fcntl.h"
#include "string.h"

void my_read(int fd)
{
	char read_buf[64];
	int len;
	int ret;
	int i;

	//get the length of data
	if(lseek(fd,0,SEEK_END)==-1)
	{
		my_err3("lseek",errno,__LINE__);
	}
	if((len=lseek(fd,0,SEEK_CUR))==-1)
	{
		my_err3("lseek",errno,__LINE__);
	}

	printf("the length of the file is:%d\n",len);
	//move the point to the start of file
	if(lseek(fd,0,SEEK_SET)==-1)
	{
		my_err3("lseek",errno,__LINE__);
	}

	if((ret=read(fd,read_buf,len))<0)
	{
		my_err3("read",errno,__LINE__);
	}
	//print the data
	printf("the string buffer:%s\n",read_buf);
	for(i=0;i<len;i++)
	{
		printf("%c",read_buf[i]);
	}
	printf("\n");
}

int main()
{
	int fd;
	char write_buf[32]="hello world!";

	//create a file
       	// if((fd=creat("example_63.c",S_IRWXU))==-1)
	if((fd=open("example_63.c",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
	{
		my_err3("creat",errno,__LINE__);
	}

	//write the data of write_buf to the file you created just now 
	if(write(fd,write_buf,strlen(write_buf))!=strlen(write_buf))
	{
		my_err3("write",errno,__LINE__);
	}

	//read the data of the file
	close(fd);
	fd=open("example_63.c",O_RDWR);
	my_read(fd);

	printf("-----------\n");

	//move the file point
	if((lseek(fd,10,SEEK_END))==-1)
	{
		my_err3("lseek",errno,__LINE__);
	}

	//write the data to the file again
	if(write(fd,write_buf,strlen(write_buf))!=strlen(write_buf))
	{
		my_err3("write",errno,__LINE__);
	}

	//read again
	my_read(fd);
	close(fd);
	return 0;
}
