#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/mutex.h> 
#include <linux/seq_file.h>

#define SUPER_BLOCKS_ADDRESS 0xc159866c  //find the varl super_blocks address from /proc/kallsyms
#define SB_LOCK_ADDRESS      0xc186cda0  // find the varl sb_lock address from /proc/kallsyms

extern struct dentry *myfs_create_file(const char *name, mode_t mode,
					struct dentry *parent, void *data,

					const struct file_operations *fops);

extern void myfs_remove(struct dentry *dentry);

static struct dentry *my_first_file;

static int sbinfo_show(struct seq_file *m, void *v)	// 将信息输入到seq_file *m中去
{
	struct super_block *sb;
        struct list_head *pos;
	seq_printf(m, "struct super_block:\n");
	
	spin_lock((spinlock_t *)SB_LOCK_ADDRESS);
	
	list_for_each(pos, (struct list_head *)SUPER_BLOCKS_ADDRESS) {
                sb = list_entry(pos, struct super_block, s_list);
		seq_printf(m, "dev_t:%d:%d file_type_name:%s\n",
				MAJOR(sb->s_dev),
				MINOR(sb->s_dev),
				sb->s_type->name);
        }
	
	spin_unlock((spinlock_t *)SB_LOCK_ADDRESS);
        return 0;
}


static int sb_open_file(struct inode *inode, struct file *filp)
{
	return single_open(filp, sbinfo_show, NULL);
}
static ssize_t sb_read_file(struct file *file, char __user *buf,
                                 size_t count, loff_t *ppos)
{
	return seq_read(file,buf,count,ppos);
}

struct file_operations sb_fops = {
	.open = sb_open_file,
	.read = sb_read_file,
};


static __init int myfile_init(void)
{
       my_first_file =  myfs_create_file("super_block", 0644, NULL, NULL, &sb_fops);
	if(!my_first_file)
		return -ENOENT;
	return 0;
}

static void __exit myfile_exit(void)
{
	myfs_remove(my_first_file);
	return;
}

module_init(myfile_init);
module_exit(myfile_exit);

MODULE_LICENSE("GPL");

