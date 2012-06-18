#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/uaccess.h>


#define MAJOR_NUM 249
#define DEV_NAME "edsionte_cdev"
#define DATA_SIZE 100

static int count=0;
static char globalmem[DATA_SIZE];
static spinlock_t lock;


static ssize_t myspin_read(struct file*flip,char *buf,size_t count,loff_t*ops)
{
	if(copy_to_user(buf,globalmem,sizeof(globalmem)))
	{
		printk("copy to user failed..\n");
		return -1;
	}
	return sizeof(globalmem);
}


static ssize_t myspin_write(struct file*flip,char *buf,size_t count,loff_t*ops)
{
	if(copy_from_user(globalmem,buf,sizeof(globalmem)))
	{
		printk("copy from user failed..\n");
		return -1;
	}

	return sizeof(globalmem);
}

static int myspin_open(struct inode*inode,struct file*flip)
{
	spin_lock(&lock);
	if(count)
	{
		printk("the spin is lock..\n");
		spin_unlock(&lock);
		return -1;
	}

	count++;
	spin_unlock(&lock);
	return 0;
}

static int myspin_release(struct inode*inode,struct file*flip)
{
	count--;
	return 0;
}

static struct file_operations fops=
{
	.read=myspin_read,
	.write=myspin_write,
	.open=myspin_open,
	.release=myspin_release,
};


static int __init myspin_init(void)
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
	
	spin_lock_init(&lock);

	return ret;
}

static void __exit myspin_exit(void)
{
	unregister_chrdev(MAJOR_NUM,DEV_NAME);
	printk("device has been unregistered..\n");

}

module_init(myspin_init);
module_exit(myspin_exit);
MODULE_LICENSE("GPL");
