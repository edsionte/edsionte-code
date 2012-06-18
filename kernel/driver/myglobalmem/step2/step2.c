#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#define GLOBALMEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define GLOBALMEM_MAJOR 0/*预设的globalmem的主设备号*/

static int globalmem_major=GLOBALMEM_MAJOR;
/*globalmem设备结构体*/
struct globalmem_dev
{
	struct cdev cdev;
	unsigned char mem[GLOBALMEM_SIZE];/*全局内存*/
};

struct globalmem_dev* globalmem_devp;

//文件操作结构体
static const struct file_operations globalmem_fops;

void globalmem_setup_cdev(struct globalmem_dev *dev,int index)
{
	int err;
	int devno=MKDEV(globalmem_major,index);

	cdev_init(&dev->cdev,&globalmem_fops);
	dev->cdev.owner=THIS_MODULE;
	dev->cdev.ops=&globalmem_fops;
	err=cdev_add(&dev->cdev,devno,1);
	if(err)
	{
		printk(KERN_NOTICE "ERROR %d adding LED%D",err,index);
	}
}

/*字符设备驱动加载函数*/
static int __init globalmem_init(void)
{
	int result;
	dev_t devno=MKDEV(globalmem_major,0);

	/*申请设备号*/
	if(globalmem_major)
	{
		result=register_chrdev_region(devno,1,"globalmem");
	}
	else
	{
		result=alloc_chrdev_region(&devno,0,1,"globalmem");
		globalmem_major=MAJOR(devno);
	}
	if(result<0)
	{
		return result;
	}

	/*动态申请设备结构体的内存*/
	globalmem_devp=kmalloc(sizeof(struct globalmem_dev),GFP_KERNEL);
	if(!globalmem_devp)
	{
		result=-ENOMEM;
		goto fail_malloc;
	}

	memset(globalmem_devp,0,sizeof(struct globalmem_dev));

	globalmem_setup_cdev(globalmem_devp,0);
	return 0;
fail_malloc: unregister_chrdev_region(devno,1);
	return result;
}

static void __exit globalmem_exit(void)
{
	cdev_del(&globalmem_devp->cdev);
	kfree(globalmem_devp);
	unregister_chrdev_region(MKDEV(globalmem_major,0),1);
}

MODULE_LICENSE("GPL");
module_init(globalmem_init);
module_exit(globalmem_exit);
