#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");

static int __init t_init(void)
{
	struct task_struct *cur = current;
	struct task_struct *proc = NULL;
	struct list_head *pos = NULL;

	list_for_each(pos, &(cur->tasks)) {
		proc = list_entry(pos, struct task_struct, tasks);

		printk("%s\t%d\n", proc->comm, proc->pid);
	}

	return 0;
}

static void __exit t_exit(void)
{
	printk("leaving..\n");
}

module_init(t_init);
module_exit(t_exit);
