#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
MODULE_LICENSE("GPL");
static int irq = 1;
static char *devname = "init_name";
module_param(irq, int, S_IRWXU);
module_param(devname, charp, S_IRWXU);

static struct work_struct mywk;
static int devid = 1112;


static void wk_handle(struct work_struct *mywk)
{
	printk("work is running..\n");

}
static irqreturn_t irq_handle(int irq, void *dev)
{
	printk("irq_handle is running..\n");
	INIT_WORK(&mywk, wk_handle);
	schedule_work(&mywk);
	return IRQ_HANDLED;
}
static int __init wk_init(void)
{
	if (request_irq(irq, irq_handle, IRQF_SHARED, devname, &devid) != 0) 
	{
		return -1;
	}

	printk("devname:%s..\n", devname);
	return 0;
}

static void __exit wk_exit(void)
{
	free_irq(irq, &devid);

}

module_init(wk_init);
module_exit(wk_exit);
