#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

#define  DEV_NAME "edsionte_cdev"
#define  DATA_SIZE 100
#define  MAJOR_NUM 250

static char globalmem[DATA_SIZE];
struct semaphore sem;

static ssize_t mysync_read(struct file*,char*,size_t,loff_t*);
static ssize_t mysync_write(struct file*,char*,size_t,loff_t*);

static struct file_operations fops=
{
	.read=mysync_read,
	.write=mysync_write,
};



static ssize_t mysync_read(struct file* flip,char *buf,size_t count,loff_t *ops)
{
	if(down_interruptible(&sem)<0)
	{
		return -1;
	}
	if(copy_to_user(buf,&globalmem,sizeof(globalmem)))
	{
		return -1;
	}

	return sizeof(globalmem);
}

static ssize_t mysync_write(struct file* flip,char *buf,size_t count,loff_t *ops)
{
	if(copy_from_user(&globalmem,buf,sizeof(globalmem)))
	{
		return -1;
	}

	up(&sem);
	return sizeof(globalmem);
}

static int __init mysync_init(void)
{
	int ret;
	dev_t devno=MKDEV(MAJOR_NUM,0);

	ret=register_chrdev(MAJOR_NUM,DEV_NAME,&fops);
	if(ret)
	{
		printk("register failed..\n");
	}
	else
	{
		printk("register sucess..\n");
	}

	init_MUTEX_LOCKED(&sem);

	return ret;
}

static void __exit mysync_exit(void)
{
	unregister_chrdev(MAJOR_NUM,DEV_NAME);
	printk("unregister success..\n");
}

module_init(mysync_init);
module_exit(mysync_exit);
MODULE_LICENSE("GPL");
