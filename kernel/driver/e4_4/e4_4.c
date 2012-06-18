#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

static char *book_name="dissecting linux Device Driver";
static int num=4000;

static int book_init(void)
{
	printk(KERN_INFO "book name:%s\n",book_name);
	printk(KERN_INFO "book num:%d\n",num);
	return 0;
}

static void book_exit(void)
{
	printk(KERN_ALERT "Book module exit\n");
}

module_init(book_init);
module_exit(book_exit);
module_param(num,int,S_IRUGO);
module_param(book_name,charp,S_IRUGO);

