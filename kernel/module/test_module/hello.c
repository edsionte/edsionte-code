#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

//必选
//模块许可声明
MODULE_LICENSE("GPL");

int edsionte_1;
static int edsionte_2;


static int hello2(void)
{
	printk(KERN_ALERT "hello2,I am edsionte\n");
	return 0;
}

int hello3(void)
{
	printk(KERN_ALERT "hello3,I am edsionte\n");
	return 0;
}
//模块加载函数
static int hello_init(void)
{
	printk(KERN_ALERT "hello,I am edsionte\n");
	hello2();
	hello3();
	return 0;
}

//模块卸载函数
static void __exit hello_exit(void)
{
	printk(KERN_ALERT "goodbye,kernel\n");
}

//模块注册
module_init(hello_init);
module_exit(hello_exit);

//可选
MODULE_AUTHOR("edsionte Wu");
MODULE_DESCRIPTION("This is a simple example!\n");
MODULE_ALIAS("A simplest example");
