#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int irq;
static char *devname;
char mydev[] = "te_dev";

module_param(irq, int, S_IRWXU);
module_param(devname, charp, S_IRWXU);

static irqreturn_t myirq_handler(int irq, void *dev);

static int __init param_interrupt_init(void)
{
	if (request_irq(irq, myirq_handler, IRQF_SHARED, devname, &mydev) != 0) {
		printk("request irq failed..\n");
		return -1;
	}

	printk("irq %d request success..\n", irq);

	return 0;
}

static void __exit param_interrupt_exit(void)
{
	free_irq(irq, &mydev);
	printk("irq %d free..\n", irq);
}

static irqreturn_t myirq_handler(int irq, void *dev)
{
	char *tdev;

	tdev = (char *)dev;
	printk("param_irq's handle is working..\n");
	printk("The irq %d's unique id is %s\n", irq, tdev);

	return IRQ_HANDLED;
}

MODULE_LICENSE("GPL");
module_init(param_interrupt_init);
module_exit(param_interrupt_exit);

