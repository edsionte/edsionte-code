#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/fdtable.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");


int pid = 1;
static int print_process_file_init(void)
{
	struct task_struct *mypcb = NULL;
	struct fdtable *myfdtb = mypcb->files->fdt;
	int i;
	int maxfd = myfdtb->max_fds;
	struct file *tmpfile = NULL;

	printk("my module is working..\n");

	if (!(mypcb = pid_task(find_vpid(pid), PIDTYPE_PID))) {
		printk("Can not find the task by pid %d..\n", pid);
	}

	for (i = 0; i < maxfd; i++) {
		tmpfile = myfdtb->fd[i];
		printk("file %lu\n", tmpfile->f_dentry->d_inode->i_ino);
	}

	printk("mypcb = %p\n", mypcb);
	return 0;
}

static void print_process_file_exit(void)
{
	printk("my module is leaving..\n");
}

module_param(pid, int, S_IRWXU);
module_init(print_process_file_init);
module_exit(print_process_file_exit);
