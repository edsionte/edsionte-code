#include <linux/init.h>
#include <linux/module.h>

//必选部分
//模块许可证声明
MODULE_LICENSE("GPL");

//模块加载函数
static int __init hello_init(void)
{
	printk("Dear Kernel,I am edsionte!\n");
	return 0;
}

//模块卸载函数
static void __exit hello_exit(void)
{
	printk("Goodbye,Honey kernel...\n");
}

//模块注册
module_init(hello_init);
module_exit(hello_exit);

//可选部分
MODULE_AUTHOR("edsionte Wu");
MODULE_DESCRIPTION("This is a simple example!\n");
MODULE_ALIAS("Say hi to kernel");
