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

#define GLOBALMEM_SIZE	0x1000	/*全局内存最大为4kb*/
#define MEM_CLEAR 0x1  /*清空全局内存*/
#define GLOBALMEM_MAJOR 247   /*预设的globalmem的主设备号*/

static globalmem_major = GLOBALMEM_MAJOR;
static struct semaphore sem;

/*globalmem设备结构体*/
struct globalmem_dev                                     
{                                                        
  struct cdev cdev; /*cedv结构体*/                       
  unsigned char mem[GLOBALMEM_SIZE]; /*全局内存*/        
};

struct globalmem_dev *globalmem_devp; /*设备结构体指针*/

/*文件打开函数*/
int globalmem_open(struct inode *inode, struct file *filp)
{
  /*将设备结构体指针赋值给文件私有数据指针*/
  down_interruptible(&sem);
  filp->private_data = globalmem_devp;
  up(&sem);
  return 0;
}
/*文件释放函数*/
int globalmem_release(struct inode *inode, struct file *filp)
{
  return 0;
}


/*读函数*/
static ssize_t globalmem_read(struct file *filp, char __user *buf, size_t size,
  loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct globalmem_dev *dev = filp->private_data; /*获得设备结构体指针*/

  /*分析获取有效的写长度*/
  if (p >= GLOBALMEM_SIZE)
    return count ?  - ENXIO: 0;
  if (count > GLOBALMEM_SIZE - p)
    count = GLOBALMEM_SIZE - p;

  if(down_interruptible(&sem))
  {
	  printk("The process quit because the data is accessing now..\n");
	  return -ERESTARTSYS;
  }

  /*从内核空间向用户空间写数据*/
  if (copy_to_user(buf, (void*)(dev->mem + p), count))
  {
    ret =  - EFAULT;
  }
  else
  {
//	  sleep(10);
    *ppos += count;
    ret = count;
 
    printk(KERN_INFO "read %d bytes(s) from %d\n", count, p);
  }

  up(&sem);/*释放信号量*/

  return ret;
}

/*写函数*/
static ssize_t globalmem_write(struct file *filp, const char __user *buf,
  size_t size, loff_t *ppos)
{
  unsigned long p =  *ppos;
  unsigned int count = size;
  int ret = 0;
  struct globalmem_dev *dev = filp->private_data; /*获得设备结构体指针*/
  
  /*分析和获取有效的写长度*/
  if (p >= GLOBALMEM_SIZE)
    return count ?  - ENXIO: 0;
  if (count > GLOBALMEM_SIZE - p)
    count = GLOBALMEM_SIZE - p;
  
  if(down_interruptible(&sem))
  {
	  printk("The process quit because the data is accessing now..\n");
	  return -ERESTARTSYS;
  }

  /*从用户空间向内核空间写数据*/
  if (copy_from_user(dev->mem + p, buf, count))
    ret =  - EFAULT;
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "written %d bytes(s) from %d\n", count, p);
  }

  up(&sem);/*释放信号量*/
  
  return ret;
}

/*文件操作结构体*/
static const struct file_operations globalmem_fops =
{
  .owner = THIS_MODULE,
  .read = globalmem_read,
  .write = globalmem_write,
  .open = globalmem_open,
  .release = globalmem_release,
};

/*初始化并注册cdev*/
static void globalmem_setup_cdev(struct globalmem_dev *dev, int index)
{
  int err, devno = MKDEV(globalmem_major, index);

  cdev_init(&dev->cdev, &globalmem_fops);
  dev->cdev.owner = THIS_MODULE;
  dev->cdev.ops = &globalmem_fops;
  err = cdev_add(&dev->cdev, devno, 1);
  if (err)
    printk(KERN_NOTICE "Error %d adding LED%d", err, index);
}

/*设备驱动模块加载函数*/
int globalmem_init(void)
{
  int result;
  //前12位为主设备号，后20位为次设备号
  dev_t devno = MKDEV(globalmem_major, 0);

  /* 申请设备号*/
  /*已知起始设备的设备号*/
  if (globalmem_major)
    result = register_chrdev_region(devno, 1, "globalmem");
  else  /* 动态申请设备号*/
  {
    result = alloc_chrdev_region(&devno, 0, 1, "globalmem");
    globalmem_major = MAJOR(devno);
  }  
  if (result < 0)
    return result;
    
  /* 动态申请设备结构体的内存*/
  globalmem_devp = kmalloc(sizeof(struct globalmem_dev), GFP_KERNEL);
  if (!globalmem_devp)    /*申请失败*/
  {
    result =  - ENOMEM;
    goto fail_malloc;
  }
  memset(globalmem_devp, 0, sizeof(struct globalmem_dev));
  
  globalmem_setup_cdev(globalmem_devp, 0);

  init_MUTEX(&sem);/*初始化信号量*/
  
  return 0;

  fail_malloc: unregister_chrdev_region(devno, 1);
  return result;
}

/*模块卸载函数*/
void globalmem_exit(void)
{
  cdev_del(&globalmem_devp->cdev);   /*注销cdev,即从系统中删除此设备*/
  kfree(globalmem_devp);     /*释放设备结构体内存*/
  unregister_chrdev_region(MKDEV(globalmem_major, 0), 1); /*释放设备号*/
}

module_param(globalmem_major, int, S_IRUGO);

module_init(globalmem_init);
module_exit(globalmem_exit);
