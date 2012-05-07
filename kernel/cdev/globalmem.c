#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>


#define GLOBALMEM_SIZE 0x100 /*the size of globalmem*/
#define MEM_CLEAR 0x1 /*clear globalmem*/
#define GLOBALMEM_MAJOR 251 /*the predefined globalmem's major devno*/

static int globalmem_major = GLOBALMEM_MAJOR;
/*define a struct of globalmem*/
struct globalmem_dev
{
	struct cdev cdev; /*cdev struct which the kernel has defined*/
	unsigned char mem[GLOBALMEM_SIZE]; /*globalmem memory*/
};

struct globalmem_dev *dev; /*the pointer variable of globalmem dev*/

static int globalmem_open(struct inode *inode, struct file *fp)
{
	fp->private_data = dev;
	return 0;
}

static int globalmem_release(struct inode *inode, struct file *fp)
{
	return 0;
}

static ssize_t globalmem_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{

	unsigned long p = *pos;
	unsigned int count = size;
	int ret = 0;
	/*get the pointer of struct globalmem_dev*/
	struct globalmem_dev *dev = fp->private_data;

	if(p >= GLOBALMEM_SIZE)
		return -1;
	if(count > GLOBALMEM_SIZE)
		count = GLOBALMEM_SIZE - p;
	
	/*the copy_to_user successfully return the bytes of data that isn't read,
	  or return -1
	 */
	if(copy_to_user((void *)buf, (void *)(dev->mem + p), count) < 0){
		ret = -1;
	}
	else{
		*pos += count;
		ret = count;
	}

	printk("read %d bytes from %ld\n",count,p);

	return ret;
}

static ssize_t globalmem_write(struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
	unsigned long p = *pos;
	unsigned int count = size;
	int ret = 0;
	struct globalmem_dev *dev = fp->private_data;

	if(p > GLOBALMEM_SIZE)
		return -1;

	if(count > GLOBALMEM_SIZE - p)
		count = GLOBALMEM_SIZE - p;

	if(copy_from_user((void *)(dev->mem + p), (void *)buf, count) < 0){
		ret = -1;
	}
	else{
		*pos += count;
		ret = count;
	}

	printk("written %d bytes from %ld\n",count, p);

	return ret;
}

/*filling the globalmem's file operation interface in the struct file_operations*/
static const struct file_operations globalmem_fops =
{
	.owner = THIS_MODULE,
//	.llseek = globalmem_llseek,
	.read = globalmem_read,
	.write = globalmem_write,
//	.ioctl = globalmem_ioctl,
	.open = globalmem_open,
	.release = globalmem_release,
};

static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
	int ret;
	int devno = MKDEV(globalmem_major, index);

	cdev_init(&dev->cdev, &globalmem_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &globalmem_fops;
	ret = cdev_add(&dev->cdev, devno, 1);
	if(ret){
		printk("adding globalmem error");
	}
}

/*module loading function*/
static int __init globalmem_init(void)
{
	int ret;
	dev_t devno;

	printk("globalmem is starting..\n");
	devno = MKDEV(globalmem_major,0);
	if(globalmem_major){
		/*predefined a globalmem's major devno*/
		ret = register_chrdev_region(devno, 1, "edsionte_globalmem");
	}
	else{
		/*not predefined a major devno and then allocate a devno*/
		ret = alloc_chrdev_region(&devno, 0, 1, "edsionte_globalmem");
		/*get the globalmem's major devno*/
		globalmem_major = MAJOR(devno);
	}

	if(ret < 1){
		return ret;
	}

	/*allocate a piece of memory for struct globalmem_dev*/
	dev = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
	if(!dev)
	{
		ret = -1;
		goto fail_malloc;
	}
	memset(dev, 0, sizeof(struct globalmem_dev));

	globalmem_setup_cdev(dev, 0);

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return ret;
}

/*module unloading function*/
static void __exit globalmem_exit(void)
{
	/*free struct cdev*/
	cdev_del(&dev->cdev);
	/*free the memory of struct globalmem_dev*/
	kfree(dev);
	/*free the devno*/
	unregister_chrdev_region(MKDEV(globalmem_major,0), 1);
}

MODULE_LICENSE("GPL");
module_init(globalmem_init);
module_exit(globalmem_exit);

