#include <linux/module.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/semaphore.h>
#include <linux/sched.h> //CLONE_KERNEL

MODULE_AUTHOR("edsionte WU");
MODULE_LICENSE("GPL");

#define NTHREADS 1000 //线程数量
//edsionte 无工作队列

struct share_struct {
	struct list_head list;
	int id;
	int pid;
};

static struct timer_list mytimer;
static LIST_HEAD(sharelist); //定义sharelist头节点
static unsigned int list_len = 0; //链表节点个数 
static DECLARE_MUTEX(sem);
static spinlock_t slock = SPIN_LOCK_UNLOCKED; //用于保护链表
static atomic_t id = ATOMIC_INIT(0);
static long count = 0;
int tid[NTHREADS] = {0};

static int sharelist_func(void *data);
static void timer_task(unsigned long data);

static int sharelist_func(void *data)
{
	struct share_struct *p = NULL;
  	int tnum = *((int *)data);

	if (count++ %4 == 0)
		printk("\n");

	spin_lock(&slock);
	if (list_len < 300) {
		if ((p = kmalloc(sizeof(struct share_struct), GFP_KERNEL)) == NULL)
			return -ENOMEM;

		p->id = atomic_read(&id); //记录每次添加的次数
		atomic_inc(&id);
		p->pid = current->pid;
		list_add(&p->list, &sharelist);
		list_len++;
		printk("Thread[%d] add:%-5d\t", tnum, p->id);
	}
	/*
	} else if (list_len > 50){
		struct share_struct *q = NULL;
		q = list_entry(sharelist.prev, struct share_struct, list); //从尾部删除节点
		list_del(sharelist.prev);
		list_len--;
		printk("Thread[%d] del:%-5d\t", tnum, q->id);
		kfree(q);
	}
*/
	spin_unlock(&slock);
	return 0;
}

//定时器超时时要调用的函数
static void timer_task(unsigned long data)
{
	if (!list_empty(&sharelist)) {
		struct share_struct *cur = NULL;
		
		if (count++ % 4 == 0)
			printk("\n");
		cur = list_entry(sharelist.next, struct share_struct, list);//获得头结点后的第一个节点
		list_del(sharelist.next);
		list_len--;
		printk("Timer[%lu] del:%-5d\t", jiffies, cur->id);
		kfree(cur);
	}

	mod_timer(&mytimer, jiffies + 1);
}

static int __init sharelist_init(void)
{
	int i;

	printk("sharelist module is running..\n");

	init_timer(&mytimer);
	mytimer.expires = jiffies;
	mytimer.data = 0;
	mytimer.function = timer_task;
	add_timer(&mytimer);

	for ( i = 0; i < NTHREADS; i++) {
		tid[i] = i;
		kernel_thread(sharelist_func, (void *)&tid[i], CLONE_KERNEL | SIGCHLD);
	}

	return 0;
}

static void __exit sharelist_exit(void)
{
	struct list_head *pos = NULL, *n = NULL;
	struct share_struct *cur_node = NULL;

	printk("sharelist module is leaving..\n");

	del_timer(&mytimer);

	spin_lock(&slock);
	list_for_each_safe(pos, n, &sharelist) {
		if (count++ % 4 == 0)
			printk("\n");
		cur_node = list_entry(pos, struct share_struct, list);
		list_del(pos);
		printk("syscall del:%d\t", cur_node->id);
		kfree(cur_node);
	}
	spin_unlock(&slock);
	printk("sharelist module is stop..\n");
}

module_init(sharelist_init);
module_exit(sharelist_exit);
