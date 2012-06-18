#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

MODULE_AUTHOR("Edsionte Wu");
MODULE_LICENSE("GPL");

#define MYCDEV_MAJOR 231 /*the predefined mycdev's major devno*/
#define MYCDEV_SIZE 100
extern struct file_operations mycdev_fops;
/*module loading function*/
static int __init mycdev_init(void)
{
	int ret;

	printk("mycdev module is staring..\n");

	ret=register_chrdev(MYCDEV_MAJOR,"edsionte_cdev",&mycdev_fops);
	if(ret<0)
	{
		printk("register failed..\n");
		return 0;
	}
	else
	{
		printk("register success..\n");
	}

	return 0;
}

/*module unloading function*/
static void __exit mycdev_exit(void)
{
	printk("mycdev module is leaving..\n");
	unregister_chrdev(MYCDEV_MAJOR,"edsionte_cdev");
}

module_init(mycdev_init);
module_exit(mycdev_exit);

