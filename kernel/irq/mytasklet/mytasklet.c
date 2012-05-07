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

//define a devid
static int mydev=1119;
static int irq;
static char* devname=NULL;

//define arguments for this module
module_param(irq,int,0644);
module_param(devname,charp,0644);

//define a argument of tasklet struct
static struct tasklet_struct mytasklet;

static void mytasklet_handler(unsigned long data)
{
	printk("This is tasklet handler..\n");
}

static irqreturn_t myirq_handler(int irq,void* dev)
{
	static int count=0;
	if(count<10)
	{
		printk("-----------%d start--------------------------\n",count+1);
            	printk("The interrupt handeler is working..\n");
             	printk("The most of interrupt work will be done by following tasklet..\n");
            	tasklet_init(&mytasklet,mytasklet_handler,0);
          	tasklet_schedule(&mytasklet);
             	printk("The top half has been done and bottom half will be processed..\n");
	}
	count++;
      	return IRQ_HANDLED;
}

static int __init mytasklet_init(void)
{
	//request a irq
	printk("My module is working..\n");
	if(request_irq(irq,myirq_handler,IRQF_SHARED,devname,&mydev)!=0)
	{
		printk("tasklet_init:can not request irq %d for %s..",irq,devname);
		return -1;
	}
	printk("%s request irq:%d success..\n",devname,irq);
	return 0;
}

static void __exit mytasklet_exit(void)
{
	printk("My module is leaving..\n");
	free_irq(irq,&mydev);
	printk("Free the irq %d..\n",irq);
}

module_init(mytasklet_init);
module_exit(mytasklet_exit);
MODULE_LICENSE("GPL");
