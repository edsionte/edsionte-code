#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>

unsigned long pagemem;
void *kmallocmem;
unsigned char *vmallocmem;

MODULE_LICENSE("GPL");

static int __init mmshow_init(void)
{
	printk("mmshow module is working\n");

	//返回unsigned long型数据
	pagemem = __get_free_page(GFP_KERNEL);
	if(!pagemem)
		goto gfp_fail;
 	printk(KERN_INFO "pagemem = 0x%lx\n",pagemem);

	//返回void*指针
	kmallocmem = kmalloc(100 * sizeof(char),GFP_KERNEL);
	if(!kmallocmem)
		goto kmalloc_fail;
	printk(KERN_INFO "kmallocmem = 0x%p\n",kmallocmem);

	//返回void*指针
	vmallocmem = vmalloc(1000000 * sizeof(char));
	if(!vmallocmem)
		goto vmalloc_fail;
	printk(KERN_INFO "vmallocmem = 0x%p\n",vmallocmem);

	return 0;

gfp_fail:
	free_page(pagemem);
kmalloc_fail:
	kfree(kmallocmem);
vmalloc_fail:
	vfree(vmallocmem);

	return -1;
}


static void __exit mmshow_exit(void)
{
	free_page(pagemem);
	kfree(kmallocmem);
	vfree(vmallocmem);

	printk("mmshow module is leaving\n");
}


module_init(mmshow_init);
module_exit(mmshow_exit);
