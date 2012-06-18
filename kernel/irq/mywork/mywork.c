/*
The inspiration of this code belongs to kerneltravel.net
Last modified by edsionte

usage: 1.Compiling this file by make 
       2.Then insert the module generated just now by the following command:
         sudo insmod filename.ko irq=1 devname=myirq

This interrupt shared the irq 1 with keyboard
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
static int irq;
static char* devname;
struct work_struct mywork;

module_param(irq,int,0644);
module_param(devname,charp,0644);

struct myirq
{
	int devid;
};
struct myirq mydev={1119};

static void mywork_handler(struct work_struct *mywork)
{
	printk("mywork is wroking..\n");
}

static irqreturn_t myirq_handler(int irq,void* dev)
{
	struct myirq mydev;
	static int count=0;
	mydev=*(struct myirq*)dev;
	printk("key:%d..\n",count+1);
	printk("devid:%d ISR is working..\n",mydev.devid);
	printk("Bottom half will be working..\n");
	INIT_WORK(&mywork,mywork_handler);
	schedule_work(&mywork);
	printk("ISR is leaving..\n");
	count++;
	return IRQ_HANDLED;
}

static int __init myirq_init(void)
{
	printk("Module is working..\n");
	if(request_irq(irq,myirq_handler,IRQF_SHARED,devname,&mydev)!=0)
	{
		printk("%s request IRQ:%d failed..\n",devname,irq);
		return -1;
	}
	printk("%s rquest IRQ:%d success..\n",devname,irq);
	return 0;
}

static void __exit myirq_exit(void)
{
	printk("Module is leaving..\n");
	free_irq(irq,&mydev);
	printk("%s request IRQ:%d success..\n",devname,irq);
}



module_init(myirq_init);
module_exit(myirq_exit);
MODULE_LICENSE("GPL");


