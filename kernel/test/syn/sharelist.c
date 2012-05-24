#include <linux/config.h>
#include <linux/version.h>
#include <linux/module.h>
#if defined(MODVERSIONS)
#include <linux/modversions.h>
#endif
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/tqueue.h>
#include <linux/wait.h>
#include <linux/signal.h>
#include <asm/semaphore.h>
#include <asm/smplock.h>
#include <linux/list.h>
#include <linux/string.h>
#include <asm/errno.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <asm/atomic.h>

//thread number
#define NTHREADS 1000 
struct my_struct{
       struct list_head list;
       int id;
       int pid;
};
wait_queue_head_t queue;
//for tq_timer 
struct tq_struct task;
//for keventd
struct tq_struct tq;
//our sharelist head
static LIST_HEAD(mine);
static unsigned int list_len = 0;
//for synchronization of kernel thread to continue runing
static DECLARE_MUTEX(sem);
//for protecting our list 
static spinlock_t my_lock=SPIN_LOCK_UNLOCKED;
//for add in atomic way
static atomic_t my_count=ATOMIC_INIT(0);

/* it build a kernel thread */
int sharelist(void*);
static void kthread_launcher(void *data)
{ 
  kernel_thread((int (*)(void *))sharelist, NULL, 0);
  up(&sem);  
}

/* the kernel thread , which add node to list */
int sharelist(void * data)
{
 
  struct my_struct *p;
  if((p=kmalloc(sizeof(struct my_struct),GFP_KERNEL))==NULL)
  return -ENOMEM;
  p->id=atomic_read(&my_count);
  atomic_inc(&my_count);
  p->pid=current->pid;
  //lock list ,stop bh
  spin_lock_bh(&my_lock);
  if(list_len<800)
    {
     list_add(&p->list,&mine);
     list_len++;
     printk("ADD:%d\t",p->id);
    }
    else
    {   
     struct my_struct *my = NULL;     
     my=list_entry(mine.prev,struct my_struct,list);
     list_del(mine.prev);
     printk("DEL:%d\t",my->id);
     kfree(my);	    
     list_len--;
    } 
    //unlock list,open bh
    spin_unlock_bh(&my_lock);
  return 0;
}

//it call keventd to run kernel thread
void start_kthread()
{
  tq.sync=0;
  INIT_LIST_HEAD(&tq.list);
  tq.routine=kthread_launcher;
  tq.data=NULL;
  schedule_task(&tq); 
  //block till kernel thread start
  down(&sem);  
}

static int count =0;
//timer qt for del node from list
int qt_task(){
	
 if(++count<1200)
 {
  if(!list_empty(&mine))
   {	       
    struct my_struct *i;
    i=list_entry(mine.next,struct my_struct,list);
    list_del(mine.next);
    printk("del:%d\t",i->id);
    kfree(i);
   }
 queue_task(&task,&tq_timer);
 }
 return 0;
}
// insmod call it first
static int  __init share_init(void)
{
  //init data	
  init_waitqueue_head(&queue);
  task.routine=(void*)qt_task;
  task.data=NULL;
  queue_task(&task,&tq_timer);
  int i;
  //to build thread
  for(i=0;i<NTHREADS;i++)
     {	  
       start_kthread();      
     }  
 return 0;
}

//uninstall module , and delete list
static void __exit share_exit(void)
{
    /* print to dmesg */
    printk("\n");	
    printk("Exiting \n");
    struct list_head *p=NULL;
    struct my_struct *my=NULL;
    //lock list ,stop bh
    spin_lock_bh(&my_lock);
    list_for_each(p,&mine)
    {
     my=list_entry(p,struct my_struct,list);
     list_del(p);
     kill_proc(my->pid,SIGKILL,1);
     printk("node is:%d\t",my->id);
     kfree(my);
    }
    //unlock list ,open bh
    spin_unlock_bh(&my_lock);
    printk("\n");
    printk("Over \n");
    //wait some time for kernel thread terminate.
    interruptible_sleep_on_timeout(&queue,HZ);
    kill_proc(2,SIGCHLD,1);
    return ;
}




module_init(share_init);/**<\brief on loading this file, call audit_init*/
module_exit(share_exit);/**<\brief on finishing with this file, call audit_exit*/
