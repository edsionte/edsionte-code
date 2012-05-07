#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int irq = 1;
static char *devname = "init_name";
module_param(irq, int, S_IRWXU);
module_param(devname, charp, S_IRWXU);

static int devid = 1111;
static struct tasklet_struct mytsk;

static void tsklet_handler(unsigned long data)
{
	printk("hi, tasklet's handler is running..\n");
}

static irqreturn_t myirq_handler(int irq, void *devid)
{
	int devid_t = *((int *)devid);

	printk("irq %d of %d is running..\n", devid_t, irq);
	tasklet_init(&mytsk, tsklet_handler, 0);
	tasklet_schedule(&mytsk);

	return IRQ_HANDLED;
}

static int __init tsklet_init(void)
{
	if (request_irq(irq, myirq_handler, IRQF_SHARED, devname, &devid) != 0) {
		printk("request irq failed..\n");
		return -1;
	}

	printk("request irq %s of %d success..\n", devname, irq);
	return 0;
}

static void __exit tsklet_exit(void)
{
	free_irq(irq, &devid);
	printk("free irq success..\n");
}

module_init(tsklet_init);
module_exit(tsklet_exit);
MODULE_LICENSE("GPL");
