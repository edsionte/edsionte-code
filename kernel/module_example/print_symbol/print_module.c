#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("edsionte Wu");

/* 
   该地址值通过以下命令得到：
   cat /proc/kallsyms | grep [^_]modules$
   注意每次重新启动内核后，都必须重新获得该地址
*/
#define modules 0xc0771264

static void print_symbol(unsigned int num_syms, const struct kernel_symbol *syms)
{
	int i;
	
	for (i = 0; i < num_syms; i++) {
		printk("%x %s\n", syms[i].value, syms[i].name);
	}
}

static int __init print_module_init(void)
{
	struct module *p;
	struct list_head *module_head;
	struct list_head *pos;
	int i = 0;

	printk("print_module module is starting..\n");

	module_head = (struct list_head *)modules;
	list_for_each(pos, module_head) {
		p = list_entry(pos, struct module, list);
		printk("%3d %20s %10d\n", ++i, p->name, p->core_size);
		print_symbol(p->num_syms, p->syms);
                printk("---------------\n");
	}
	
	return 0;
}

static void __exit print_module_exit(void)
{
	printk("print_module is leaving..\n");
}

module_init(print_module_init);
module_exit(print_module_exit);
