#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/dcache.h>
#include <linux/namei.h>
#include <linux/path.h>

#define PATH_LOOKUP_ADDRESS 0xc01c7fd0 
#define FILENAME "/home/susu/lks"

static int __init linktest_init(void)
{
	struct nameidata nd;
	struct dentry *d_pos;
	struct inode *inode;
	int error = 0;

	printk("linktest init ...............\n");
	error = path_lookup(FILENAME, LOOKUP_CONTINUE, &nd);
	if(error){
		goto out;
	}

	inode = nd.path.dentry->d_inode;
	list_for_each_entry(d_pos, &(inode->i_dentry), d_alias){
		printk("inode_num:%lx, dentry_name::%s\n", d_pos->d_inode->i_ino,d_pos->d_name.name);
	}
out:
	return error;
}

static void linktest_exit(void)
{
	printk("linktest exit...............\n");
}

MODULE_LICENSE("GPL");
module_init(linktest_init);
module_exit(linktest_exit);
