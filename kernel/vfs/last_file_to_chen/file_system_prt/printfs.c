#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/kdev_t.h>

#define FILE_SYSTEM_ADDRESS  0xc05b2a60 //find the varl super_blocks address from /proc/kallsyms
#define FILE_SYSTEM_LOCK_ADDRESS 0xc048de40 // find the varl sb_lock address from /proc/kallsyms 

static int __init auditfs_init(void)
{
	struct file_system_type **pos;

	printk("file_systems:%d\n",sizeof(struct file_system_type));
	printk("\nprint file_sys_type:\n");
	
	read_lock((rwlock_t *)FILE_SYSTEM_LOCK_ADDRESS);
	pos=(struct file_system_type **)FILE_SYSTEM_ADDRESS;
	while(*pos) {
		printk("name:%s\n",(*pos)->name);
		pos = &(*pos)->next;
	}

	read_unlock((rwlock_t *)FILE_SYSTEM_LOCK_ADDRESS);
	
	return 0;
}

static void __exit auditfs_exit(void)
{
	printk("unloading....\n");
}

module_init(auditfs_init);
module_exit(auditfs_exit);
MODULE_LICENSE("GPL");

