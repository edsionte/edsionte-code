#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/semaphore.h>
#include <linux/sched.h>

#define BUF_NUM 5
#define N 5
#define PNUM 3
#define CNUM N*PNUM

static int index[N];
struct semaphore s1;
struct semaphore s2;
struct semaphore mutex;
static int buf[BUF_NUM];
static int in,out;
static int cflag;

int productor_thread(void *p)
{
	int i=*(int *)p;
	int p_num=PNUM;

	while(p_num)
	{
		if((s1.count)<=0)
		{
			printk("[producer %d,%d]:I will be waiting for producting..\n",i,s1.count);
		}
		down(&s1);
	        down(&mutex);
          	buf[in]=i*100+(PNUM-p_num+1);
          	printk("[producer %d,%d]:I producted a goods \"%d\" to buf[%d]..\n",i,s1.count,buf[in],in);
            	in=(in+1)%BUF_NUM;
         	up(&mutex);
          	up(&s2);
		p_num--;
	}
	return 0;
}

int consumer_thread(void *p)
{
	int i=*(int *)p;
	int goods;

	while(cflag!=CNUM)
	{
		if((s2.count)<=0)
		{
			printk("[consumer %d,%d]:I will be waiting for goods..\n",i,s2.count);
		}
	        down(&s2);
            	down(&mutex);
          	goods=buf[out];
           	printk("[consumer %d,%d]:I consumed a goods \"%d\" from buf[%d]..\n",i,s2.count,goods,(out%BUF_NUM));
           	out=(out+1)%BUF_NUM;
            	up(&mutex);
            	up(&s1);
		cflag++;
	}
	return 0;
	
}

static int __init np_nc_init(void)
{
	int i;

	printk("np_nc module is working..\n");
	in=out=0;
	cflag=0;
	init_MUTEX(&mutex);
	sema_init(&s1,BUF_NUM);
	sema_init(&s2,0);

	for(i=0;i<N;i++)
	{
		index[i]=i+1;
		kernel_thread(productor_thread,&(index[i]),CLONE_KERNEL);
		kernel_thread(consumer_thread,&(index[i]),CLONE_KERNEL);
	}

	return 0;
}

static void __exit np_nc_exit(void)
{
	printk("np_nc module is leaving..\n");
}

module_init(np_nc_init);
module_exit(np_nc_exit);
MODULE_LICENSE("GPL");
