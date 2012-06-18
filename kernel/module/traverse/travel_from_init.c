#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");

static int __init  traverse_init(void)
{
      struct task_struct *p = NULL;
      struct list_head *init_head, *pos;
      int count = 0;
      
      printk("Traversal module is working..\n");
      
      init_head = &(init_task.tasks);
      list_for_each(pos, init_head) {
	      p = list_entry(pos, struct task_struct, tasks);
	      count++;
              printk("[process %d]: %s\'s pid is %d\n",
			      count, p->comm, p->pid);
      }

      /* another way of traversing all processes */
      /*
      list_for_each_entry(pos, init_head, tasks)
      {
             count++;
             printk("[process %d]: %s\'s pid is %d\n",count, pos->comm, pos->pid);
      }
      */

      printk(KERN_ALERT"The number of process is: %d\n", count);
      return 0;
}

static void __exit traverse_exit(void)
{
  printk("Traversal module is leaving..\n");
}

module_init(traverse_init);
module_exit(traverse_exit);
