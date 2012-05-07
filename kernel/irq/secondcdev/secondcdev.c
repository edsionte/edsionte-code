#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#define SECOND_MAJOR 250//second device's major device number

static int second_major=SECOND_MAJOR;


//the second device struct
struct second_dev
{
	struct cdev cdev;//cdev struct
	atomic_t counter;
	struct timer_list s_timer;//the timer used by the device
};

struct second_dev *second_devp;//the point of the second device struct

//the timer's handler function
static void second_timer_handler(unsigned long arg)
{
	mod_timer(&second_devp->s_timer,jiffies+HZ);
	atomic_inc(&second_devp->counter);
	printk("Current jiffies is %ld\n",jiffies);
}

//open function
static int second_open(struct inode*inode,struct file*filp)
{
	//initializing the timer
	init_timer(&second_devp->s_timer);
	second_devp->s_timer.function=&second_timer_handler;
	second_devp->s_timer.expires=jiffies+HZ;
	add_timer(&second_devp->s_timer);//add a timer
	atomic_set(&second_devp->counter,0);//clear the counter
	return 0;
}

//release function
static int second_release(struct inode*inode,struct file*filp)
{
	del_timer(&second_devp->s_timer);
	return 0;
}

//read function
static ssize_t second_read(struct file *filp,char __user *buf,size_t count,loff_t *ppos)
{
	int counter;

	counter=atomic_read(&second_devp->counter);
	if(put_user(counter,(int*)buf))
	{
		return -EFAULT;
	}
	else
	{
		return sizeof(unsigned int);
	}

}

//struct file_operations
static const struct file_operations second_fops=
{
	.owner=THIS_MODULE,
	.open=second_open,
	.release=second_release,
	.read=second_read,
};

static void second_setup_cdev(struct second_dev *dev,int index)
{
	int err,devno=MKDEV(second_major,index);

	//initialization
	cdev_init(&dev->cdev,&second_fops);
	dev->cdev.owner=THIS_MODULE;
	dev->cdev.ops=&second_fops;
	//register
	err=cdev_add(&dev->cdev,devno,1);
	if(err)
	{
		printk("ERROR:%d adding LED:%d\n",err,index);
	}
}

static __init int second_init(void)
{
	int ret;
	dev_t devno=MKDEV(second_major,0);//get the full device number

	//apply for the device number
	if(second_major)
	{
		//static application
		ret=register_chrdev_region(devno,1,"second_character_device");
	}
	else
	{
		//dynamic application
		ret=alloc_chrdev_region(&devno,0,1,"second_character_device");
		second_major=MAJOR(devno);
	}
	if(ret<0)
	{
		//application failed
		return ret;
	}
	//dynamic apply for memory for second device struct
	second_devp=kmalloc(sizeof(struct second_dev),GFP_KERNEL);
	if(!second_devp)
	{
		//application failed
		ret=-ENOMEM;
		goto fail_malloc;
	}

	memset(second_devp,0,sizeof(struct second_dev));
	//initializing and registering character device
	second_setup_cdev(second_devp,0);
	return 0;
fail_malloc:unregister_chrdev_region(devno,1);
	
}

static void __exit second_exit(void)
{
	cdev_del(&second_devp->cdev);//unregister character device
	kfree(second_devp);//free the memory of second device struct
	unregister_chrdev_region(MKDEV(second_major,0),1);//free the device number
}

module_init(second_init);
module_exit(second_exit);
MODULE_LICENSE("GPL");




