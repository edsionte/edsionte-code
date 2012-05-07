#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <asm/page.h>
#include <asm/pgtable.h>

MODULE_LICENSE("GPL");

static int pid = 1;
static unsigned long vaddr = 0x0;

module_param(vaddr, ulong, S_IRWXU);
module_param(pid, int, S_IRWXU);

static int __init v2p_init(void)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long paddr = 0;
	struct task_struct *mypcb = NULL;

	printk("v2p module is running..\n");
	//打印一些宏
	printk("PAGE_OFFSET = 0x%lx\n", PAGE_OFFSET);
	printk("PGDIR_SHIFT = %d\n", PGDIR_SHIFT);
	printk("PUD_SHIFT = %d\n", PUD_SHIFT);
	printk("PMD_SHIFT = %d\n", PMD_SHIFT);
	printk("PAGE_SHIFT = %d\n", PAGE_SHIFT);

	printk("PTRS_PER_PGD = %d\n", PTRS_PER_PGD);
	printk("PTRS_PER_PUD = %d\n", PTRS_PER_PUD);
	printk("PTRS_PER_PMD = %d\n", PTRS_PER_PMD);
	printk("PTRS_PER_PTE = %d\n", PTRS_PER_PTE);

	printk("PAGE_MASK = 0x%lx\n", PAGE_MASK);

	//通过pid返回进程描述符
	if (!(mypcb = pid_task(find_vpid(pid), PIDTYPE_PID))) {
		printk("can't find the task by %d\n", pid);
		return -1;
	}

	//判断指定的va是否合法
	if (!find_vma(mypcb->mm, vaddr)) {
		printk("vaddr 0x%lx not available\n", vaddr);
		return -1;
	}

	pgd = pgd_offset(mypcb->mm, vaddr);
	printk("pgd_val = 0x%lx\n", pgd_val(*pgd));
	if (pgd_none(*pgd)) {
		printk("not mapped in pgd\n");
		return -1;
	}

	pud = pud_offset(pgd, vaddr);
	printk("pud_val = 0x%lx\n", pud_val(*pud));
	if (pud_none(*pud)) {
		printk("not mapped in pud\n");
		return -1;
	}

	pmd = pmd_offset(pud, vaddr);
	printk("pmd_val = 0x%lx\n", pmd_val(*pmd));
	if (pmd_none(*pmd)) {
		printk("not mapped in pmd\n");
		return -1;
	}

	pte = pte_offset_map(pmd, vaddr);
	printk("pte_val = 0x%lx\n", pte_val(*pte));
	if (pte_none(*pte)) {
		printk("not mapped in pte\n");
		return -1;
	}

	//页框物理地址机制 | 偏移量
	paddr = (pte_val(*pte) & PAGE_MASK) | (vaddr & ~PAGE_MASK);
	printk("vaddr 0x%lx in RAM is 0x%lx\n", vaddr, paddr);
	
	return 0;
}

static void __exit v2p_exit(void)
{
	printk("v2p module is leaving..\n");
}

module_init(v2p_init);
module_exit(v2p_exit);
