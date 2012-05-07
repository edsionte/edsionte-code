#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define __GLOBALMEM_
#include "globalmem.h"

MODULE_LICENSE("Dual BSD/GPL");

struct globalmem_dev dev;
unsigned int globalmem_major = GLOBALMEM_MAJOR;

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
out:
	return retval;
}
static void __exit globalmem_exit(void)
{
	dev_t devno;

	devno = MKDEV(globalmem_major, 0);
	unregister_chrdev_region(devno, 1);
}
module_init(globalmem_init);
module_exit(globalmem_exit);
