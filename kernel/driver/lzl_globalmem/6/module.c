#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#define __GLOBALMEM_
#include "globalmem.h"

MODULE_LICENSE("Dual BSD/GPL");

struct globalmem_dev *devp;
unsigned int globalmem_major = GLOBALMEM_MAJOR;

static int globalmem_setup_cdev(struct globalmem_dev *devp, int index)
{
	int err = 0;
	dev_t devno = MKDEV(globalmem_major, index);

	cdev_init(&devp->cdev, &globalmem_fops);
	devp->cdev.owner = THIS_MODULE;
	devp->cdev.ops = &globalmem_fops;
	err = cdev_add(&devp->cdev, devno, 1);
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
		retval = register_chrdev_region(devno, 2, GLOBALMEM_NAME);
	} else {
		retval = alloc_chrdev_region(&devno, 0, 2, GLOBALMEM_NAME);
		globalmem_major = MAJOR(devno);
	}
	if (retval < 0) {
		printk(KERN_WARNING"Cannot request major %u\n", globalmem_major);
		goto out;
	}
	devp = kmalloc(sizeof(struct globalmem_dev) * 2, GFP_KERNEL);
	if (!devp) {
		printk(KERN_WARNING"alloc dev kernel memory fail\n");
		retval = -ENOMEM;
		goto fail_malloc;
	}
	memset(devp, 0, 2 * sizeof(struct globalmem_dev));

	retval = globalmem_setup_cdev(&devp[0], 0);
	retval = globalmem_setup_cdev(&devp[1], 1);
	if (retval < 0) {
		printk(KERN_WARNING"Adding device major %u\n", globalmem_major);
		goto out;
	}
	return 0;
fail_malloc:
	unregister_chrdev_region(devno, 2);
out:
	return retval;
}
static void __exit globalmem_exit(void)
{
	dev_t devno = MKDEV(globalmem_major, 0);

	cdev_del(&(devp[0].cdev));
	cdev_del(&(devp[1].cdev));
	kfree(devp);
	unregister_chrdev_region(devno, 2);
}
module_init(globalmem_init);
module_exit(globalmem_exit);
