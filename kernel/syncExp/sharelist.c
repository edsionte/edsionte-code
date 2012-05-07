#include <linux/init.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <linux/sched.h>

#define THREAD_NUM 1000  //kernel thread's num
struct sharelist {
	struct list_head list;
	int id;
	int pid;
};

wait_queue_head_t queue;
struct work_struct timer_wk; //this workqueue executed by timer;
struct work_struct keventd_wk; //this workqueue executed by keventd(kernel thread);

static LIST_HEAD(mine); //the head of sharelist
static unsigned int list_len = 0;
static DECLARE_MUTEX(sem);
static spinlock_t my_lock = SPIN_LOCK_UNLOCKED;
static atomic_t my_count = ATOMIC_INIT(0);
static int timer_over = 0;

int sharelist(void *);
void start_kthread();
void kthread_launcher(void *);
int timer_task();

//the function executed by kernel thread(keventd);
int sharelist(void *data)
{
	struct sharelist *p;

	if ((p = kmalloc(sizeof(struct sharelist), GFP_KERNEL)) == NULL)
		return -ENOMEM;

	p->id = atomic_read(&my_count);
	atomic_inc(&my_count);
	p->pid = current->pid;
	spin_lock(&my_lock);
	if (list_len < 100) {
		list_add(&p->list, &mine);
		list_len++;
		printk("ADD:%d\t", p->id);
	} else {
		struct sharelist *p = NULL;
		p = list_entry(mine.prev, struct sharelist, list);
		list_entry(mine.prev, struct sharelist, list);
		list_len--;
		printk("DEL:: %d\t", p->id);
		kfree(p);
	}

	spin_unlock(&my_lock);
	return 0;
}


void start_kernel()
{
	INIT_LIST_HEAD(&keventd_wk.entry);
	keventd_wk.func = kthread_launcher;
//	atomic_set(keventd_wk.data, 0);
	schedule_work(&keventd_wk);
	down(&sem);
}

void kthread_launcher(void *data)
{
	kernel_thread(sharelist, NULL, CLONE_KERNEL);
	up(&sem);
}


int timer_task()
{
	if (timer_over == 0) {
		if (!list_empty(&mine)) {
			struct sharelist *p;
			p = list_entry(mine.next, struct sharelist, list);
			list_del(mine.next);
			list_len--;
			printk("del:%d\t", p->id);
			kfree(p);
		}

	//	queue_task(&task, &tq_timer);
	}
}

static int __init sharelist_init(void) 
{
	printk("sharelist module is working..\n");
	init_waitqueue_head(&queue);
	timer_wk.func = timer_task;
	//task.data = 0;
//	queue_task(&task, &)

	int i;
	for (i = 0; i < THREAD_NUM; i++) {
		start_kthread();
	}

	return 0;
}

static void __exit sharelist_exit(void) 
{
	printk("sharelist module is exiting..\n");
	struct list_head *p;
	struct sharelist *tmpnode;
	spin_lock_bh(&my_lock);
	list_for_each(p, &mine) {
		tmpnode = list_entry(p, struct sharelist, list);
		list_del(p);
		kill_proc(tmpnode->pid, SIGKILL, 1);
		printk("node is: %d\t", tmpnode->id);
		kfree(tmpnode);
	}

	spin_unlock_bh(&my_lock);
	printk("all node has been removed from sharelist..\n");
	interruptible_sleep_on_timeout(&queue, HZ * 10);
	kill_proc(2, SIGCHLD, 1);
}
