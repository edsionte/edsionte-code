#include <linux/init.h>
#include <linux/module.h>
#include <asm/uaccess.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>

static unsigned long addr = 0x0;

static void find_myvma(void)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	printk("find the vma..\n");

	if (addr == 0) {
		printk("addr is 0x0..\n");
		return ;
	}

	down_read(&mm->mmap_sem);
	vma=find_vma(mm, addr);
	/* when the vma_start > address, it means that the address doesn't belong to the vma */
	if(vma && addr <= vma->vm_start)
	{
		printk("address 0x%lx found in the VMA:0x%lx-0x%lx ",addr,vma->vm_start,vma->vm_end);
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

		if(vma->vm_flags & VM_SHARED)
			printk("s");
		else
			printk("p");
		printk("\n");
	}
	else
	{
		printk("address 0x%lx didn't find in the VMA which you just now found..\n",addr);
	}
	up_read(&mm->mmap_sem);
}

static int __init mymem_init(void)
{
	printk("mymem module is working..\n");

	find_myvma();

	return 0;
}

static void __exit mymem_exit(void)
{
	printk("mymem module is leaving..\n");
}

MODULE_LICENSE("GPL");
module_init(mymem_init);
module_exit(mymem_exit);
module_param(addr, ulong, S_IRUGO);
