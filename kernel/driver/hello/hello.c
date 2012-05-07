#include <linux/module.h>
#include <linux/kernel.h>
//必选
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "hello,I am edsionte\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "goodbye,kernel\n");
}

module_init(hello_init);
module_exit(hello_exit);
//可选
MODULE_AUTHOR("edsionte Wu");
MODULE_DESCRIPTION("This is a simple example!\n");
MODULE_ALIAS("A simplest example");
