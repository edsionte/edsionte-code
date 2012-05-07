#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/kdev_t.h>

/*
 * The following value by the command:
 * 1) grep super_blocks /proc/kallsyms
 * 2) grep " sb_lock" /proc/kallsyms 
 * and,you must get the two new values again when you restarted your PC;
 */
#define SUPER_BLOCKS_ADDRESS 0xc0772a30
#define SB_LOCK_ADDRESS 0xc08c9d60

static int __init my_init(void)
{
	struct super_block *sb;
	struct list_head *pos;
	struct list_head *linode;
	struct list_head *ldentry;
	struct inode *pinode;
	struct dentry *pdentry;
	unsigned long long count = 0;

	printk("print some fields of super blocks:\n");
	spin_lock((spinlock_t *)SB_LOCK_ADDRESS);
	
	/*SUPER_BLOCKS_ADDRESS is the head point of the super_block list */
	list_for_each(pos, (struct list_head *)SUPER_BLOCKS_ADDRESS){
		sb = list_entry(pos, struct super_block, s_list);
		printk("dev_t:%d,%d ",MAJOR(sb->s_dev),MINOR(sb->s_dev));
		printk("fs_name:%s\n",sb->s_type->name);

		list_for_each(linode, &sb->s_inodes){
			pinode = list_entry(linode, struct inode, i_sb_list);
			count ++;
			printk("%lu[",pinode->i_ino);

			list_for_each(ldentry, &pinode->i_dentry){
				pdentry = list_entry(ldentry, struct dentry, d_alias);
				printk("%s->",pdentry->d_name.name);
			}
			printk("]\t");
		}
		
		printk("\n");
	}

	spin_unlock((spinlock_t *)SB_LOCK_ADDRESS);
	printk("the number of inodes:%llu\n",sizeof(struct inode)*count);

	return 0;
}

static void __exit my_exit(void)
{
	printk("unloading..\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");

