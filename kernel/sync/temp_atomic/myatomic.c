#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <linux/fs.h>

#define DATA_SIZE 100
#define DEV_NAME "edsionte_cdev"
#define MAJOR_NUM 249

static int count=0;
static spinlock_t lock;
static char globalmem[DATA_SIZE];

static int myspin_open(struct inode*inode,struct file*filp)
{
	spin_lock(&lock);

	if(count)
	{
		printk("I am wating..\n");
		return -1;
	}

	count++;
	spin_unlock(&lock);
	
	return 0;
}

static int myspin_release(struct inode*inode,struct file*filp)
{
	count--;
	return 0;
}

static ssize_t myspin_read(struct file*filp,char*buf,size_t count,loff_t*ops)
{
	if(copy_to_user(buf,globalmem,sizeof(globalmem))<0)
	{
		printk("copy to user failed..\n");
		return -1;
	}

	return sizeof(globalmem);
}

static ssize_t myspin_write(struct file*filp,char*buf,size_t count,loff_t*ops)
{
	if(copy_from_user(globalmem,buf,sizeof(globalmem))<0)
	{
		printk("copy from user failed..\n");
		return -1;
	}
	return sizeof(globalmem);
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

	return ret;

}

static void __exit myspin_exit(void)
{
	unregister_chrdev(MAJOR_NUM,DEV_NAME);
	printk("unregister success...\n");
}

module_init(myspin_init);
module_exit(myspin_exit);
MODULE_LICENSE("GPL");

