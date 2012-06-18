#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>

#include "globalmem.h"

extern struct globalmem_dev *devp;
int globalmem_open(struct inode *inode, struct file *filp)
{
	filp->private_data = devp;
	return 0;
}
int globalmem_release(struct inode *inode, struct file *filp)
{
	return 0;
}
ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos)
{
	struct globalmem_dev *dev = filp->private_data;
	unsigned int p = *ppos;
	int ret = 0;

	if (p >= GLOBALMEM_SIZE)
		return count ? -ENXIO : 0;
	if (p + count > GLOBALMEM_SIZE)
		count = GLOBALMEM_SIZE - p;
	if (copy_to_user(buf, (void *)(dev->mem + p), count))
		ret = -EFAULT;
	else {
		*ppos += count;
		ret = count;
	}

	return ret;
}
ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos)
{
	struct globalmem_dev *dev = filp->private_data;
	unsigned int p = *ppos;
	int ret = 0;

	if (p >= GLOBALMEM_SIZE)
		return count ? -ENXIO : 0;
	if (p + count > GLOBALMEM_SIZE)
		count = GLOBALMEM_SIZE - p;
	if (copy_from_user(dev->mem + p, buf, count))
		ret = -EFAULT;
	else {
		*ppos += count;
		ret = count;
	}

	return ret;
}
loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	switch (orig) {
		case 0:
			if (offset < 0) {
				ret = -EINVAL;
				break;
			}
			if ((unsigned int)offset > GLOBALMEM_SIZE) {
				ret = -EINVAL;
				break;
			}
			filp->f_pos = (unsigned int)offset;
			ret = filp->f_pos;
			break;
		case 1:
			if ((filp->f_pos + offset) > GLOBALMEM_SIZE) {
				ret = -EINVAL;
				break;
			}
			if ((filp->f_pos + offset) < 0) {
				ret = -EINVAL;
				break;
			}
			filp->f_pos = filp->f_pos + offset;
			break;
		case 2:
			if (offset > 0) {
				ret = -EINVAL;
				break;
			}
			if ((offset + GLOBALMEM_SIZE) < 0) {
				ret = -EINVAL;
				break;
			}
			filp->f_pos += offset;
			break;
		default:
			ret = -EINVAL;
	}
	return ret;
}
int globalmem_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct globalmem_dev *dev = filp->private_data;

	switch (cmd) {
		case MEM_CLEAR:
			memset(dev->mem, 0, GLOBALMEM_SIZE);
			printk(KERN_INFO"globalmem is set to zero\n");
			break;
		default:
			return -EINVAL;
	}
	return 0;
}
