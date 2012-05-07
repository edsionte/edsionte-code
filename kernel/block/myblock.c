#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUM 249
#define BUF_NUM 100

static char globalmem[BUF_NUM];
static wait_queue_head_t rdwait;
static wait_queue_head_t wrwait;
static struct semaphore mutex;

static int len;
ssize_t myblock_read(struct file*,char*,size_t count,loff_t*);
ssize_t myblock_write(struct file*,char*,size_t count,loff_t*);

struct file_operations fops=
{
	.read=myblock_read,
	.write=myblock_write,
};

ssize_t myblock_read(struct file*fp,__user char *buf,size_t count,loff_t*offp)
{
	int ret;
	DECLARE_WAITQUEUE(wait,current);

	down(&mutex);
	add_wait_queue(&rdwait,&wait);

	while(len==0)
	{
		__set_current_state(TASK_INTERRUPTIBLE);
		up(&mutex);
		schedule();
		if(signal_pending(current))
		{
			ret=-1;
			goto signal_out;
		}

		down(&mutex);
	}

	if(count>len)
	{
		count=len;
	}

	if(copy_to_user(buf,globalmem,count)==0)
	{
		memcpy(globalmem,globalmem+count,len-count);
		len-=count;
		printk("read %d bytes\n",count);
		wake_up_interruptible(&wrwait);
		ret=count;
	}
	else
	{
		ret=-1;
		goto copy_err_out;
	}

copy_err_out:up(&mutex);
signal_out:remove_wait_queue(&rdwait,&wait);

	set_current_state(TASK_RUNNING);
	return ret;
}

ssize_t myblock_write(struct file*fp,char*buf,size_t count,loff_t*offp)
{
	int ret;
	DECLARE_WAITQUEUE(wait,current);

	down(&mutex);
	add_wait_queue(&wrwait,&wait);

	while(len==BUF_NUM)
	{
		__set_current_state(TASK_INTERRUPTIBLE);
		up(&mutex);
		schedule();
		if(signal_pending(current))
		{
			ret=-1;
			goto signal_out;
		}

         	down(&mutex);
	}
	if(count>(BUF_NUM-len))
	{
		count=BUF_NUM-len;
	}

	if(copy_from_user(globalmem+len,buf,count)==0)
	{
		len=len+count;
		printk("written %d bytes\n",count);
		wake_up_interruptible(&rdwait);
		ret=count;
	}
	else
	{
		ret=-1;
		goto COPY_ERR_OUT;
	}

signal_out:up(&mutex);
COPY_ERR_OUT:remove_wait_queue(&wrwait,&wait);
	set_current_state(TASK_RUNNING);

	return ret;


}


static int __init myblock_init(void)
{
	int ret;

	printk("myblock module is working..\n");

	ret=register_chrdev(MAJOR_NUM,"edsionte_block_cdev",&fops);
	if(ret<0)
	{
		printk("register failed..\n");
		return 0;
	}
	else
	{
		printk("register success..\n");
	}
	init_MUTEX(&mutex);
	init_waitqueue_head(&rdwait);
	init_waitqueue_head(&wrwait);

	return 0;
}

static void __exit myblock_exit(void)
{
	printk("myblock module is leaving..\n");
	unregister_chrdev(MAJOR_NUM,"edsionte_block_cdev");
}

module_init(myblock_init);
module_exit(myblock_exit);
MODULE_LICENSE("GPL");
