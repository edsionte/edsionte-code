#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define __GLOBALMEM_
#include "globalmem.h"

MODULE_LICENSE("Dual BSD/GPL");

struct globalmem_dev dev;
unsigned int globalmem_major = GLOBALMEM_MAJOR;

static int globalmem_setup_cdev(dev_t devno, unsigned int minor)
{
	int err = 0;

	cdev_init(&dev.cdev, &globalmem_fops);
	dev.cdev.owner = THIS_MODULE;
	dev.cdev.ops = &globalmem_fops;
	err = cdev_add(&dev.cdev, devno, minor);
	if (err)
		printk(KERN_NOTICE"Error %d adding globalmem\n", err);
	printk(KERN_INFO"adding device major %d\n", globalmem_major);
	return err;
}

static int __init globalmem_init(void)
{
	dev_t devno = MKDEV(globalmem_major, 0);
	int retval = 0;
	
	if (globalmem_major) {
		retval = register_chrdev_region(devno, 1, GLOBALMEM_NAME);
	} else {
		retval = alloc_chrdev_region(&devno, 0, 1, GLOBALMEM_NAME);
		globalmem_major = MAJOR(devno);
	}
	if (retval < 0) {
		printk(KERN_WARNING"Cannot request major %u\n", globalmem_major);
		goto out;
	}
	retval = globalmem_setup_cdev(devno, 1);
	if (retval < 0) {
		printk(KERN_WARNING"Adding device major %u\n", globalmem_major);
		goto out;
	}
out:
	return retval;
}
static void __exit globalmem_exit(void)
{
	dev_t devno;

	devno = MKDEV(globalmem_major, 0);
	unregister_chrdev_region(devno, 1);
	cdev_del(&dev.cdev);
}
module_init(globalmem_init);
module_exit(globalmem_exit);
