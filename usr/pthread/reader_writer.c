/**
*读者写者问题
*问题描述
*1)允许多位读者同时访问某数据，但是同一时间，只允许一位写者写入数据
*2)当没有读者在读取数据的时候，才允许写者写入数据
*3)当有写者正在写入数据的时候，不允许读者进行数据的读写
*History:
*2010/03/16	huangwei	First release
*/

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define 	READER_MAX	3		/*最多允许多少人一起读取数据*/
#define		WRITER_MAX	2		/*最多允许多少人写数据*/

pthread_rwlock_t	rw_lock;		/*读写锁*/

/**
*reader_thread - 模拟读者进行数据读取
*@arg :  没有使用
*/

void* reader_thread(void *arg)
{
	while(1) {
		/*尝试加只读锁*/		
		if(pthread_rwlock_tryrdlock(&rw_lock)) {
			/*加只读锁不成功，打印信息，1秒钟后继续尝试*/
			printf("读\t者%u暂时不能读取数据.\n", (unsigned int)pthread_self());
			sleep(1);
		} else {
			/*加只读锁成功，显示哪位读者正在进行数据读取*/
			printf("读\t者%u正在读取数据.\n", (unsigned int)pthread_self());
			sleep(1);	/*读者正在读取数据*/
			printf("读\t者%u读取数据完毕.\n", (unsigned int)pthread_self());
			pthread_rwlock_unlock(&rw_lock); /*解只读锁*/
			sleep(2);	/*休息一段时间后再尝试进行数据读取*/
		}	
	}
}

/**
*writer_thread -  模拟写者写入数据
*@arg : 没有使用
*/

void* writer_thread(void *arg)
{
	while(1) {
		/*尝试加写锁*/
		if(pthread_rwlock_trywrlock(&rw_lock)) {
			/*加写锁不成功，输出信息，2秒后再继续尝试*/
			printf("写者%u暂时不能写入数据\n", (unsigned int)pthread_self());
			sleep(2);
		} else {
			/*加写锁成功，显示目前哪为写者正在进行数据写入*/
			printf("写者%u正在写入数据.\n", (unsigned int)pthread_self());
			sleep(2);	/*写者正在写入数据*/
			printf("写者%u写入数据完毕.\n", (unsigned int)pthread_self());
			pthread_rwlock_unlock(&rw_lock);
			sleep(3);	/*休息一段时间后再尝试进行数据写入*/
		}
	}
}

int main(int argc, char* argv[])
{
	pthread_t reader, writer;		/*记录读者和写者的线程号*/
	
	int i = 0;				/*循环变量*/
	
	pthread_rwlock_init(&rw_lock, NULL);	/*初始化读写锁*/
	
	/*创建READER_MAX个读者*/
	for(i = 0; i < READER_MAX; i++)	
		pthread_create(&reader, NULL, (void *)reader_thread, NULL);

	/*创建WRITER_MAX个读者*/
	for(i = 0; i < WRITER_MAX; i++)
		pthread_create(&writer, NULL, (void *)writer_thread, NULL);
	
	sleep(10);				/*程序运行10秒后退出*/
	
	return 0;
}
