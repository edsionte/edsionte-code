#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/kdev_t.h>

#define FILE_SYSTEM_ADDRESS 0xc08ca3a4 /* grep file_systems /proc/kallsyms */
#define FILE_SYSTEM_LOCK_ADDRESS 0xc0772de0 /* grep file_systems_lock /proc/kallsyms */

static int __init printfs_init(void)
{
	struct file_system_type **pos;

	printk("\n\nprint file_system_type:\n");

	read_lock((rwlock_t *)FILE_SYSTEM_LOCK_ADDRESS);
	pos = (struct file_system_type **)FILE_SYSTEM_ADDRESS;

	while(*pos){
		printk("name: %s\n",(*pos)->name);
		pos = &((*pos)->next);
	}

	read_unlock((rwlock_t *)FILE_SYSTEM_LOCK_ADDRESS);

	return 0;
}

static void __exit printfs_exit(void)
{
	printk("printfs_module is unloading..\n");
}

module_init(printfs_init);
module_exit(printfs_exit);
MODULE_LICENSE("GPL");
