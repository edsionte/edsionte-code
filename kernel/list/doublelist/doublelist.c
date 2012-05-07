#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Edsionte Wu");

#define N 10

//链表节点
struct numlist {
	int num;//数据
	struct list_head list;//指向双联表前后节点的指针
};

struct numlist numhead;//头节点

static int __init doublelist_init(void)
{
	//初始化头节点
	struct numlist *listnode;//每次申请链表节点时所用的指针
	struct list_head *pos;
	struct numlist *p;
	int i;

	printk("doublelist is starting...\n");
	INIT_LIST_HEAD(&numhead.list);

	//建立N个节点，依次加入到链表当中
	for (i = 0; i < N; i++) {
		listnode = (struct numlist *)kmalloc(sizeof(struct numlist), GFP_KERNEL);
		listnode->num = i+1;
		list_add_tail(&listnode->list, &numhead.list);
		printk("Node %d has added to the doublelist...\n", i+1);
	}

	//遍历链表
	i = 1;
	list_for_each(pos, &numhead.list) {
		p = list_entry(pos, struct numlist, list);
		printk("Node %d's data:%d\n", i, p->num);
		i++;
	}
	
	return 0;
}

static void __exit doublelist_exit(void)
{
	struct list_head *pos, *n;
	struct numlist *p;
	int i;
	
	//依次删除N个节点
	i = 1;
	list_for_each_safe(pos, n, &numhead.list) {
		list_del(pos);//从双联表中删除当前节点
		p = list_entry(pos, struct numlist, list);//得到当前数据节点的首地址，即指针
		kfree(p);//释放该数据节点所占空间
		printk("Node %d has removed from the doublelist...\n", i++);
	}
	printk("doublelist is exiting..\n");
}

module_init(doublelist_init);
module_exit(doublelist_exit);
