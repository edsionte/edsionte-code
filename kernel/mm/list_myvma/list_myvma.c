#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>


static void list_vma(void)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	printk("list vma..\n");

	printk("current:%s pid:%d\n",current->comm,current->pid);
	down_read(&mm->mmap_sem);
	for(vma = mm->mmap; vma; vma = vma->vm_next)
	{
		printk("0x%lx-0x%lx ",vma->vm_start,vma->vm_end);
		if(vma->vm_flags & VM_READ)
			printk("r");
		else
			printk("-");

		if(vma->vm_flags & VM_WRITE)
			printk("w");
		else
			printk("-");

		if(vma->vm_flags & VM_EXEC)
			printk("x");
		else
			printk("-");
		printk("\n");

	}
	up_read(&mm->mmap_sem);
}

static int __init mymem_init(void)
{
	printk("mymem module is working..\n");

	list_vma();

	return 0;
}

static void __exit mymem_exit(void)
{
	printk("mymem module is leaving..\n");
}

MODULE_LICENSE("GPL");
module_init(mymem_init);
module_exit(mymem_exit);
