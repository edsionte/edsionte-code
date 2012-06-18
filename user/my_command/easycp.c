#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{

int fdrd,fdwt,i,j,k, len;
char ch;

//fistly the content which was read from srouce file will be write to dest file
	if((fdrd=open(argv[1],O_RDONLY))==-1)
	{
		perror("open");
		exit(1);
	}
	if(lseek(fdrd,0,SEEK_END)==-1)
	{
		perror("lseek");
		exit(1);
	}
	if((len=lseek(fdrd,0,SEEK_CUR))==-1)
	{
		perror("lseek");
		exit(1);
	}
	if(lseek(fdrd,0,SEEK_SET)==-1)
	{
		perror("lseek");
		exit(1);
	}
	//open the dest file
	if((fdwt=open(argv[2],O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH))==-1)
	{
		perror("open");
		exit(1);
	}
	close(fdwt);

	if((fdwt=open(argv[2],O_WRONLY|O_APPEND))==-1)
	{
		perror("open");
		exit(1);
	}

	while(len-->0)
	{
		//write all characters to dest file
		if(read(fdrd,ch,1)!=1)
		{
			perror("read");
			exit(1);
		}
		if(write(fdwt,ch,1)!=1)
		{
			perror("write");
			exit(1);
		}

	}
}
