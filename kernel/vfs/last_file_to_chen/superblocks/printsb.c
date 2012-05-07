#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/kdev_t.h>
#define SUPER_BLOCKS_ADDRESS 0xc048db0c //find the varl super_blocks address from /proc/kallsyms
#define SB_LOCK_ADDRESS	     0xc05b2440 // find the varl sb_lock address from /proc/kallsyms 
static int __init auditfs_init(void)
{
	struct super_block *sb;
	struct list_head *pos;
	printk("sb:%d\tinode:%d\n",sizeof(struct super_block),sizeof(struct inode));
	printk("\nprint file_sys_type:\n");
	spin_lock((spinlock_t *)SB_LOCK_ADDRESS); 
	list_for_each(pos, (struct list_head *)SUPER_BLOCKS_ADDRESS) {
		sb = list_entry(pos, struct super_block, s_list);
		printk("dev_t:%d:%d  ", MAJOR(sb->s_dev),MINOR(sb->s_dev));
		printk("count:%d  file_type name:%s\n", sb->s_count, sb->s_type->name);
	}
	spin_unlock((spinlock_t *)SB_LOCK_ADDRESS); 
	return 0;
}

static void __exit auditfs_exit(void)
{
	printk("unloading....\n");
}

module_init(auditfs_init);
module_exit(auditfs_exit);
MODULE_LICENSE("GPL");

