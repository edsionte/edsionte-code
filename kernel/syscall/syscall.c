#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/unistd.h>
#include <linux/sched.h>

#define SYS_CALL_TABLE_ADDRESS 0xc0509940
#define NUM 223

int orig_cr0;
unsigned long *sys_call_table_my = 0;
static int (*anything_saved)(void);

static int clear_cr0(void)
{
	unsigned int cr0 = 0;
	unsigned int ret;
	
	asm volatile ("movl %%cr0, %%eax":"=a"(cr0));/* =表示只写，a表示eax寄存器 */
	ret = cr0;
	cr0 &= 0xfffeffff;
	asm volatile ("movl %%eax, %%cr0": :"a"(cr0));
	return ret;
}

static void setback_cr0(int val)
{
	asm volatile ("movl %%eax, %%cr0": : "a"(val));
}

asmlinkage long sys_mycall(void)
{
	printk("I am mycall, current->pid:%d, and current->comm:%s\n", current->pid, current->comm);
	return current->pid;
}

static int __init call_init(void)
{
	sys_call_table_my = (unsigned long*)(SYS_CALL_TABLE_ADDRESS);
	
	printk("call_init.......\n");
	
	anything_saved = (int (*)(void))(sys_call_table_my[NUM]);
	orig_cr0 = clear_cr0();
	sys_call_table_my[NUM] =(unsigned long) &sys_mycall;
	setback_cr0(orig_cr0);

	return 0;
}

static void __exit call_exit(void)
{
	printk("call_exit..........\n");
	orig_cr0 = clear_cr0();
	sys_call_table_my[NUM] = (unsigned long)anything_saved;
	setback_cr0(orig_cr0);
}

MODULE_LICENSE("GPL");
module_init(call_init);
module_exit(call_exit);
