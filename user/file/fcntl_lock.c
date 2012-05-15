#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

//set lock
int lock_set(int fd,struct flock* lock)
{
	if(fcntl(fd,F_SETLK,lock)==0)
	{
		//print the type of lock
		if(lock->l_type==F_RDLCK)
		{
			printf("set read lock, pid:%d\n",getpid());
		}
		else if(lock->l_type==F_WRLCK)
		{
			printf("set write lock,pid:%d\n",getpid());
		}
		else if(lock->l_type==F_UNLCK)
		{
			printf("release the lock,pid:%d\n",getpid());
		}
		else 
		{
			printf("set lock wrong\n");
		}
	}
	else
	{
		printf("lock operation fail\n");
		return -1;
	}
	return 0;
}

int lock_test(int fd,struct flock *lock)
{
	if(fcntl(fd,F_GETLK,lock)==0)
	{
		if(lock->l_type==F_UNLCK)
		{
			printf("lock can be set in this file\n");
		}
		else
		{
			if(lock->l_type==F_RDLCK)
			{
				printf("can't set lock,read lock has been set by:%d\n",lock->l_pid);
			}
			else if(lock->l_type==F_WRLCK)
			{
				printf("can't set lock,write lock has been set by:%d\n",lock->l_pid);
			}
			return -2;
		}
	}
	else
	{
		perror("get incompatible locks fail\n");
		return -1;
	}
}



int main()
{
	int fd;
	int ret;
	struct flock lock;
	char read_buf[32];

	//create a file
	if((fd=open("example_65.c",O_CREAT|O_TRUNC|O_RDWR,S_IRWXU))==-1)
	{
		perror("creat");
	}
	if((write(fd,"test lock!",10))!=10)
	{
		perror("write");
	}
	//initialize the struct of lock
	memset(&lock,0,sizeof(struct flock));
	//set a lock to all data of the file
	lock.l_start=SEEK_SET;
	lock.l_whence=0;
	lock.l_len=0;
	//set read lock
	lock.l_type=F_RDLCK;
	if(lock_test(fd,&lock)==0)//if the lock can be set
	{
		lock.l_type=F_RDLCK;
		lock_set(fd,&lock);
	}
        //move the point to start 
	lseek(fd,0,SEEK_SET);
	if((ret=read(fd,read_buf,10))<0)
	{
		perror("read");
	}

	read_buf[ret]='\0';
	printf("the data of this file is:%s\n",read_buf);

	//waiting for a any key
	getchar();

	//set write lock
	lock.l_type=F_WRLCK;
	if(lock_test(fd,&lock)==0)
	{
		lock.l_type=F_WRLCK;
		lock_set(fd,&lock);
	}

	//release the lock
	lock.l_type=F_UNLCK;
	lock_set(fd,&lock);
	close(fd);
	return 0;
}
