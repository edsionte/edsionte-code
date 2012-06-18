#ifndef __GLOBALMEM_H_
#define __GLOBALMEM_H_

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/errno.h>

#define GLOBALMEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 0
#define GLOBALMEM_NAME "globalmem"

struct globalmem_dev {
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];
};

#ifdef __GLOBALMEM_
extern int globalmem_open(struct inode *inode, struct file *filp);
extern int globalmem_release(struct inode *inode, struct file *filp);
extern ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos);
extern ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos);
extern loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig);
extern int globalmem_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

struct file_operations globalmem_fops = {
	.owner = THIS_MODULE,
	.read = globalmem_read,
	.write = globalmem_write,
	.open = globalmem_open,
	.release = globalmem_release,
	.llseek = globalmem_llseek,
	.ioctl = globalmem_ioctl
};
#endif /* end of __GLOBALMEM_ */
#endif /* end of __GLOBALMEM_H_ */
