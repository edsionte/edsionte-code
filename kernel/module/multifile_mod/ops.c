#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

static int mycdev_open(struct inode *inode, struct file *fp)
{
	return 0;
}

static int mycdev_release(struct inode *inode, struct file *fp)
{
	return 0;
}

static ssize_t mycdev_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
	unsigned long p = *pos;
	unsigned int count = size;
	char kernel_buf[MYCDEV_SIZE] = "This is mycdev!";

	if(p >= MYCDEV_SIZE)
		return -1;
	if(count > MYCDEV_SIZE)
		count = MYCDEV_SIZE - p;
	
	if (copy_to_user(buf, kernel_buf, count) != 0) {
		printk("read error!\n");
		return -1;
	}

	printk("edsionte's reader: %d bytes was read...\n", count);
	return count;

}

static ssize_t mycdev_write(struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
	return size;
}

/*filling the mycdev's file operation interface in the struct file_operations*/

static const struct file_operations mycdev_fops =
{
	.owner = THIS_MODULE,
	.read = mycdev_read,
	.write = mycdev_write,
	.open = mycdev_open,
	.release = mycdev_release,
};

