#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/slab.h>  //kmalloc
#include <linux/cdev.h>  
#include <linux/semaphore.h>
#include <linux/errno.h> //error codes
#include <linux/types.h> //ssize_t
#include <linux/fcntl.h> //O_ACCMODE

#include <asm/uaccess.h> //copy_to_user
#include "scull.h"

int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int scull_nr_devs = SCULL_NR_DEVS;
int scull_quantum = SCULL_QUANTUM;
int scull_qset = SCULL_QSET;

struct scull_dev *scull_devices = NULL;

struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.read = scull_read,
	.write = scull_write,
	.open = scull_open,
	.release = scull_release,
};

int scull_open(struct inode *inode, struct file *filp) 
{
	struct scull_dev *dev;

	dev = container_of(inode->i_cdev, struct scull_dev, cdev);
	filp->private_data = dev;

	if ((filp->f_flags & O_ACCMODE) == O_WRONLY) {
		scull_trim(dev);
	}

	return 0;
}

int scull_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t scull_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum;
	int qset = dev->qset;
	int itemsize = quantum * qset;//一个链表项的大小
	int item, s_pos, q_pos, rest;
	ssize_t retval = 0;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;
	if (*offp >= dev->size) {
		printk("offp over the length of file..\n");
		goto out;
	}

	if (*offp + count > dev->size) {
		printk("the reading size is too much, but it's fixed..\n");
		count = dev->size - *offp;
	}

	item = (long)*offp / itemsize;//第几个链表项
	rest = (long)*offp % itemsize;
	s_pos = rest / quantum;//在量子集中的位置
	q_pos = rest % quantum;//在量子中的位置

	printk("read:offp=%lld, item=%d, rest=%d, s_pos=%d, q_pos=%d\n", offp, item, rest, s_pos, q_pos);

	dptr = scull_follow(dev, item);

	if (dptr == NULL || !dptr->data || !dptr->data[s_pos])
		goto out;

	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_to_user(buff, dptr->data[s_pos] + q_pos, count)) {
		retval = -EFAULT;
		goto out;
	}
	
	*offp += count;
	retval = count;

out:
	up(&dev->sem);
	return retval;
}

ssize_t scull_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
	struct scull_dev *dev = filp->private_data;
	struct scull_qset *dptr;
	int quantum = dev->quantum;
	int qset = dev->qset;
	int itemsize = quantum * qset;
	int item, s_pos, q_pos, rest;
	ssize_t retval = -ENOMEM;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	item = (long)*offp / itemsize;
	rest = (long)*offp % itemsize;
	s_pos = rest / quantum;
	q_pos = rest % quantum;

	printk("write:offp=%lld, item=%d, rest=%d, s_pos=%d, q_pos=%d\n", offp, item, rest, s_pos, q_pos);
	
	dptr = scull_follow(dev, item);
	if (dptr == NULL)
		goto out;
	if (!dptr->data) {
		dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);
		if (!dptr->data)
			goto out;
		memset(dptr->data, 0, qset * sizeof(char *));
	}

	if (!dptr->data[s_pos]) {
		dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);
		if (!dptr->data[s_pos])
			goto out;
	}

	if (count > quantum - q_pos)
		count = quantum - q_pos;

	if (copy_from_user(dptr->data[s_pos] + q_pos, buff, count)) {
		retval = -EFAULT;
		goto out;
	}

	*offp += count;
	retval = count;

	if (dev->size < *offp)
		dev->size = *offp;

out:
	up(&dev->sem);
	return retval;
}

static void scull_setup_cdev(struct scull_dev *dev, int index)
{
	int err;
	dev_t devno;
	
	devno = MKDEV(scull_major, scull_minor + index);

	cdev_init(&dev->cdev, &scull_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);

	if (err)
		printk(KERN_NOTICE "Error %d adding scull%d", err, index);
}

//free all memory
int scull_trim(struct scull_dev *dev)
{
	struct scull_qset *next, *dptr;
	int qset = dev->qset;
	int i;

	for (dptr = dev->data; dptr; dptr = next) {
		if (dptr->data) {
			for (i = 0; i < qset; i++) 
				kfree(dptr->data[i]);
			kfree(dptr->data);
			dptr->data = NULL;
		}
		
		next = dptr->next;
		kfree(dptr);
	}

	dev->size = 0;
	dev->quantum = scull_quantum;
	dev->qset = scull_qset;
	dev->data = NULL;

	return 0;
}

struct scull_qset *scull_follow(struct scull_dev *dev, int n)
{
	//从链表起始出发前进n个节点
	struct scull_qset *qs = dev->data;

	if (!qs) {
		qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
		if (qs == NULL)
			return NULL;
		memset(qs, 0, sizeof(struct scull_qset));
	}

	while (n--) {
		if (!qs->next) {
			qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);
			if (qs->next == NULL)
				return NULL;
			memset(qs->next, 0, sizeof(struct scull_qset));
		}
		qs = qs->next;
		continue;
	}

	return qs;
}

static int __init scull_init(void)
{
	dev_t dev;
	int result, i;

	printk("scull module is running..\n");
	//allocate the devno
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, scull_nr_devs, "scull");
		printk("static allocate..\n");
	} else {
		result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
		scull_major = MAJOR(dev);
		printk("dynamic allocate..\n");
	}

	if (result < 0) {
		printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
		return result;
	}

	scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);
	if (!scull_devices) {
		printk(KERN_WARNING "error: kmalloc is failed\n");
		return -ENOMEM;
	}
	memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));

	//initialize each devices and register the device to kernel
	for (i = 0; i < scull_nr_devs; i++) {
		scull_devices[i].quantum = scull_quantum;
		scull_devices[i].qset = scull_qset;
		init_MUTEX(&(scull_devices[i].sem));
		scull_setup_cdev(&scull_devices[i], i);
	}
	
	return 0;
}

static void __exit scull_exit(void)
{
	int i;
	dev_t devno = MKDEV(scull_major, scull_minor);

	if (scull_devices) {
		for (i = 0; i < scull_nr_devs; i++) {
			scull_trim(scull_devices + i);
			cdev_del(&scull_devices[i].cdev);
		}
		kfree(scull_devices);
	}

	unregister_chrdev_region(devno, scull_nr_devs);

	printk("scull module is leaving..\n");
}

module_init(scull_init);
module_exit(scull_exit);
