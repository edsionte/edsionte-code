#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>

#define MODULE_VERS "1.0"
#define MODULE_NAME "edsionte_procfs"
#define FOOBAR_LEN 8

MODULE_LICENSE("GPL");

struct fb_data_t {
	char name[FOOBAR_LEN + 1];
	char value[FOOBAR_LEN + 1];
};

static struct proc_dir_entry *example_dir, *foo_file, *bar_file, *jiffies_file, *symlink;
struct fb_data_t foo_data, bar_data;

//jiffies文件对应的读函数
static int proc_read_jiffies(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "jiffies = %ld\n", jiffies);

	return len;
}

//foo和bar对应的读函数
static int proc_read_foobar(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	struct fb_data_t *fb_data = (struct fb_data_t *)data;

	//将fb_data的数据写入page
	len = sprintf(page, "%s = %s\n", fb_data->name, fb_data->value);

	return len;
}

//foo和bar对应的写函数
static int proc_write_foobar(struct file *file, const char *buffer, unsigned long count, void *data)
{
	int len;
	struct fb_data_t *fb_data = (struct fb_data_t *)data;

	if (count > FOOBAR_LEN)
		len = FOOBAR_LEN;
	else
		len = count;

	//写函数的核心语句，将用户态的buffer写入内核态的value中
	if (copy_from_user(fb_data->value, buffer, len))
		return -EFAULT;

	fb_data->value[len] = '\0';

	return len;
}

//加载函数
static int __init procfs_example_init(void)
{
	int rv = 0;

	//创建目录procfs_example
	example_dir = proc_mkdir(MODULE_NAME, NULL);
	if (example_dir == NULL) {
		rv = -ENOMEM;
		goto out;
	}

	//创建只读文件jiffies
	jiffies_file = create_proc_read_entry("jiffies", 0444, example_dir, proc_read_jiffies, NULL);
	if (jiffies_file == NULL) {
		rv = -ENOMEM;
		goto no_jiffies;
	}

	//创建可读写文件foo
	foo_file = create_proc_entry("foo", 0644, example_dir);
	if (foo_file == NULL) {
		rv = -ENOMEM;
		goto no_foo;
	}

	strcpy(foo_data.name, "foo");
	strcpy(foo_data.value, "foo");
	foo_file->data = &foo_data;
	foo_file->read_proc = proc_read_foobar;
	foo_file->write_proc = proc_write_foobar;

	//创建可读写文件bar
	bar_file = create_proc_entry("bar", 0644, example_dir);
	if (bar_file == NULL) {
		rv = -ENOMEM;
		goto no_bar;
	}

	strcpy(bar_data.name, "bar");
	strcpy(bar_data.value, "bar");
	bar_file->data = &bar_data;
	bar_file->read_proc = proc_read_foobar;
	bar_file->write_proc = proc_write_foobar;

	//为jiffies创建符号连接文件jiffies_too
	symlink = proc_symlink("jiffies_too", example_dir, "jiffies");
	if (symlink == NULL) {
		rv = -ENOMEM;
		goto no_symlink;
	}

	printk(KERN_INFO "%s %s initalised...\n", MODULE_NAME, MODULE_VERS);

	return 0;

no_symlink:
	remove_proc_entry("bar", example_dir);
no_bar:
	remove_proc_entry("foo", example_dir);
no_foo:
	remove_proc_entry("jiffies", example_dir);
no_jiffies:
	remove_proc_entry(MODULE_NAME, NULL);
out:
	return rv;
}

//卸载函数
static void __exit procfs_example_exit(void)
{
	remove_proc_entry("jiffies_too", example_dir);
	remove_proc_entry("bar", example_dir);
	remove_proc_entry("foo", example_dir);
	remove_proc_entry("jiffies", example_dir);
	remove_proc_entry("MODULE_NAME", NULL);

	printk(KERN_INFO "%s %s removed...\n", MODULE_NAME, MODULE_VERS);
}

module_init(procfs_example_init);
module_exit(procfs_example_exit);
