#include <linux/fs.h>
#include <linux/cdev.h>

#include "globalmem.h"

int globalmem_open(struct inode *inode, struct file *filp) {return 0;}
int globalmem_release(struct inode *inode, struct file *filp) {return 0;}
ssize_t globalmem_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos) {return 0;}
ssize_t globalmem_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos) {return 0;}
loff_t globalmem_llseek(struct file *filp, loff_t offset, int orig){return 0;}
int globalmem_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg) {return 0;}
