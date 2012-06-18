#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>


#define MAJOR_NUM 248
#define DEV_NAME "edsionte_sync_cdev"
#define DATA_SIZE 100

static struct semaphore sem;
static char globalmem[DATA_SIZE];


static ssize_t mysync_read(struct file*filp,char* buf,size_t count,loff_t* ops)
{
	down_interruptible(&sem);

	if(copy_to_user(buf,globalmem,sizeof(globalmem)))
	{
		printk("copy to user failed..\n");
		return -1;
	}

	return sizeof(globalmem);
}

static ssize_t mysync_write(struct file*filp,char*buf,size_t count,loff_t*ops)
{
	if(copy_from_user(globalmem,buf,sizeof(globalmem)))
	{
		printk("copy from user failed..\n");
		return -1;
	}

	up(&sem);
	return sizeof(globalmem);
}


static struct file_operations fops=
{
	.read=mysync_read,
	.write=mysync_write,
};

static int __init mysync_init(void)
{
	int ret;

	ret=register_chrdev(MAJOR_NUM,DEV_NAME,&fops);
	if(ret<0)
	{
		printk("register failed..\n");
	}
	else
	{
		printk("register success..\n");
	}

	init_MUTEX_LOCKED(&sem);
	return ret;
}


static void __exit mysync_exit(void)
{
	unregister_chrdev(MAJOR_NUM,DEV_NAME);
	printk("device has been unregister..\n");
}


module_init(mysync_init);
module_exit(mysync_exit);
MODULE_LICENSE("GPL");
