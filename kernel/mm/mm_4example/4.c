#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock_types.h>
#include <linux/proc_fs.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <asm/uaccess.h>
#include <linux/hardirq.h>

static void mtest_list_vma(void)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
		
	printk("mtest_vma_list.......\n");
		
	down_read(&mm->mmap_sem);
	for(vma = mm->mmap; vma; vma = vma->vm_next){
		printk("vma  0x%lx-0x%lx", vma->vm_start, vma->vm_end);
		if(vma->vm_flags & VM_WRITE)
			printk("	write");
		if(vma->vm_flags & VM_READ)
			printk("	read");
		if(vma->vm_flags & VM_EXEC)
			printk("	exec");
		printk("\n");
	}
	up_read(&mm->mmap_sem);
}

static void mtest_find_vma(unsigned long addr)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;

	printk("mtest_find_vma.............\n");

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, addr);
	if(vma && addr >= vma->vm_start){
		printk("address 0x%lx found in vma :0x%lx-0x%lx,	%lx\n", addr, vma->vm_start, vma->vm_end, vma->vm_flags);
	}else {
		printk("address 0x%lx not found.........\n", addr);
	}
	up_read(&mm->mmap_sem);
}

static struct page *my_follow_page(struct vm_area_struct *vma, unsigned long addr)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	spinlock_t *ptl;
	struct page *page = NULL;
	struct mm_struct *mm = vma->vm_mm;
	pgd = pgd_offset(mm, addr);
	if(pgd_none(*pgd) || unlikely(pgd_bad(*pgd))){
		goto out;
	}
	
	pud = pud_offset(pgd, addr);
	if(pud_none(*pud) || unlikely(pud_bad(*pud))){
		goto out;
	}
	
	pmd = pmd_offset(pud, addr);
	if(pmd_none(*pmd) || unlikely(pmd_bad(*pmd))){
		goto out;
	}

	pte = pte_offset_map_lock(mm, pmd, addr, &ptl);
	if(!pte){
		goto out;
	}

	if(!pte_present(*pte)){
		goto unlock;
	}

	page = pfn_to_page(pte_pfn(*pte));
	if(!page){
		goto unlock;
	}

	get_page(page);

unlock:
	pte_unmap_unlock(pte, ptl);
out:
	return page;
}

static void mtest_find_page(unsigned long addr)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	struct page *page;
	unsigned long kernel_addr;

	printk("mtest_find_page..........\n");
	
	down_read(&mm->mmap_sem);
	vma = find_vma(mm, addr);
	page = my_follow_page(vma, addr);
	if(!page){
		printk("page not found for 0x%lx\n", addr);
		goto out;
	}

	printk("page found for 0x%lx\n", addr);	

	kernel_addr = (unsigned long)page_address(page);
	printk("kernel_address_base:0x%lx\n", kernel_addr);

	kernel_addr += (addr & ~PAGE_MASK);
	printk("find 0x%lx to kernel_address 0x%lx\n", addr, kernel_addr);
out:
	up_read(&mm->mmap_sem);
}

static void mtest_write_val(unsigned long addr, unsigned long val)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	struct page *page;
	unsigned long kernel_addr;
	
	printk("mtest_write_val..............\n");	

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, addr);
	if(vma && addr >= vma->vm_start && (addr + sizeof(val)) <= vma->vm_end){
		if(!(vma->vm_flags & VM_WRITE)){
			printk("vma is not writable for 0x%lx\n", addr);
			goto out;
		}
	
		page = my_follow_page(vma, addr);
		if(!page){
			printk("page not found for 0x%lx\n", addr);
			goto out;
		}

		kernel_addr = (unsigned long)page_address(page);
		kernel_addr += (addr & ~PAGE_MASK);
	
		printk("write val:0x%lx to kernel_addr:0x%lx\n", val, kernel_addr);
		*(unsigned long *)kernel_addr = val;
		printk("write val:0x%lx to kernel_addr:0x%lx\n", val, kernel_addr);
	
		put_page(page);
	}else {
		printk("no vma found for address :0x%lx\n", addr);
	}
out:
	up_read(&mm->mmap_sem);
}

static ssize_t mtest_write(struct file *file, const char __user *buffer, size_t count, loff_t *data)
{
	char buf[128];
	unsigned long val, val2;
	
	printk("mtest_write...................\n");
	if(count > sizeof(buf))
		return -EINVAL;

	if(copy_from_user(buf, buffer, count))
		return -EINVAL;

	if(0 == memcmp(buf, "listvma", 7)){
		mtest_list_vma();
	}
	else if(0 == memcmp(buf, "findvma", 7)){
		if(1 == sscanf(buf + 7, "%lx", &val)){
			mtest_find_vma(val);
		}
	}
	else if(0 == memcmp(buf, "findpage", 8)){
		if(1 == sscanf(buf + 8, "%lx" , &val)){
			mtest_find_page(val);
		}	
	}
	else if(0 == memcmp(buf, "writeval", 8)){
		if(2 == sscanf(buf + 8, "%lx %lx",&val, &val2)){
			mtest_write_val(val, val2);
		}
	}
	return count;
}

static struct file_operations proc_mtest_operations = {
	.write = mtest_write,
};

static struct proc_dir_entry *mtest_proc_entry;

static int __init mtest_init(void)
{
	mtest_proc_entry = create_proc_entry("mtest", 0777, NULL);
	if(mtest_proc_entry == NULL){
		printk("create proc entry error\n");
		return -1;
	}
	
	printk("mtest_init successful\n");
	mtest_proc_entry->proc_fops = &proc_mtest_operations;
	return 0;
}

static void __exit mtest_exit(void)
{
	printk("mtest exit..........\n");
	remove_proc_entry("mtest", NULL);
}

MODULE_LICENSE("GPL");
module_init(mtest_init);
module_exit(mtest_exit);

