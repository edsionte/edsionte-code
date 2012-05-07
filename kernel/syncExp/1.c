#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/sched.h>
#include <asm/atomic.h>

#define NTHREADS 200 /* 线程数 */
struct sharelist {
	struct list_head list;
	int id;
	int pid;
};
static struct work_struct queue; /* 定义工作队列 */
static struct timer_list mytimer;   /* 用于定时器队列 */
static LIST_HEAD(mine);  /* sharelist头 */
static unsigned int list_len = 0; 
static DECLARE_MUTEX(sem);  /* 内核线程进行同步的信号量 */
static spinlock_t my_lock = SPIN_LOCK_UNLOCKED; /* 保护对链表的操作 */
static atomic_t my_count = ATOMIC_INIT(0); /* 以原子方式进行追加 */
static long count = 0; /* 行计数器，每行打印4个信息 */
static int timer_over = 0; /* 定时器结束标志 */

static int sharelist(void *data)
{
	struct sharelist *p;

	if ((p = kmalloc(sizeof(struct sharelist), GFP_KERNEL)) == NULL)
		return -ENOMEM;

	p->id = atomic_read(&my_count);
	atomic_inc(&my_count);
	p->pid = current->pid;

	spin_lock(&my_lock); /* 添加锁，保护共享资源 */
	if (list_len < 100) { /* 如果共享链表的长度小于100 */
      /* 申请并初始化一个节点 
     * 将新节点加入共享链表中，链表长度加1
     * 打印提示信息
       */
		list_add(&p->list, &mine);
		list_len++;
		printk("ADD:%d\t", p->id);
	} else { /* 队列超过定长则删除节点 */
      /* 从链表尾部获取一个节点
     * 删除取得的尾部节点，链表长度减1
     * 打印提示信息，释放节点
       */
		struct sharelist *p = NULL;
		p = list_entry(mine.prev, struct sharelist, list);
		list_entry(mine.prev, struct sharelist, list);
		list_len--;
		printk("DEL:: %d\t", p->id);
		kfree(p);	    
	}
	spin_unlock(&my_lock); /* 释放自旋锁 */
	return 0;
}

/* 由内核的events来运行内核线程 */
static void start_kthread(void)
{
	down(&sem);
	schedule_work(&queue);//调度工作队列
}


void kthread_launcher(struct work_struct *q)
{
	kernel_thread(sharelist, NULL, CLONE_KERNEL | SIGCHLD); //创建内核线程
	up(&sem);
}

void qt_task(unsigned long data)
{
	if (timer_over == 0)
		if (!list_empty(&mine)) {
 /* 如果共享链表不为空 */
	/* 获取共享链表最新插入的节点
 * 删除获取的节点，链表长度减1
 * 打印提示信息，释放节点
   */	
			struct sharelist *p;
			p = list_entry(mine.next, struct sharelist, list);
			list_del(mine.next);
			list_len--;
			printk("del:%d\t", p->id);
			kfree(p);
		}
}

static int __init share_init(void)
{
	int i;

	printk(KERN_INFO"share list module is working..\n");

	INIT_WORK(&queue, kthread_launcher);//初始化工作队列
	setup_timer(&mytimer, qt_task, 0); //设置定时器
	add_timer(&mytimer); //激活定时器
	for (i = 0; i < NTHREADS; i++) //再启动200个内核线程来添加节点
		start_kthread();
	
	return 0;
}

static void __exit share_exit(void)
{
	struct list_head *p, *n;
	struct sharelist *pos;

	printk("sharelist module is exiting..\n");
	
	del_timer(&mytimer); //删除定时器
	spin_lock(&my_lock); /* 上锁，以保护临界区 */
	list_for_each_safe(p, n, &mine) { 
		/* 删除所有节点，销毁链表 */		
		list_del(p);
		pos = list_entry(p, struct sharelist, list);
		kfree(pos);
	}

	spin_unlock(&my_lock); /* 开锁 */
}

module_init(share_init);
module_exit(share_exit);
