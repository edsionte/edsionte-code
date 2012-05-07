#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/security.h>

static int my_task_kill(struct task_struct *p, struct siginfo *info, int sig, u32 secid)
{
	if (SIGTERM == sig) {
		if (strcmp(p->comm, "vim")) {
			return -EPERM;
		}
	}

	return 0;
}

static struct security_operations task_ops = {
	.taks_kill = my_task_kill,
};

static int __init task_init(void) {
	printk("my task_kill module is working..\n");

	if (register_security(&task_ops)) {
		printk("register error..\n");
		return -1;
	}

	return 0;
}

static void __exit task_exit(void) {
	if (unregister_security(&task_ops)) {
		printk("unregister error..\n");
	}
}

MODULE_LICENSE("GPL");
module_init(task_init);
module_exit(task_exit);


