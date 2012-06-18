#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#define N 10

static struct semaphore mutex;
static struct semaphore apple;
static struct semaphore orange;


static int father_thread(void *p)
{
	char *name=(char *)p;
	int i=N;

	while(i--){
		down(&mutex);
         	printk("[%s]:I put %d apple on the plate\n",name,(N-i));
            	up(&apple);
	}

	return 0;
}

static int mother_thread(void *p)
{
	char *name=(char *)p;
	int i=N;

	while(i--){
		down(&mutex);
         	printk("[%s]:I put %d orange on the plate\n",name,(N-i));
            	up(&orange);	
	}

	return 0;
}


static int son_thread(void *p)
{
	char *name=(char *)p;
	int i=N;

	while(i--){
		down(&apple);
         	printk("[%s]:I take %d apple on the plate\n",name,(N-i));
            	up(&mutex);
	}

	return 0;
}

static int daughter_thread(void *p)
{
	char *name=(char *)p;
	int i=N;

	while(i--){
		down(&orange);
         	printk("[%s]:I put %d orange on the plate\n",name,(N-i));
            	up(&mutex);
	}

	return 0;
}

static int __init eatingfruit_init(void)
{
	printk("eating fruit module is working..\n");

	//creating four threads
	kernel_thread(father_thread,"father",CLONE_KERNEL);
	kernel_thread(mother_thread,"mother",CLONE_KERNEL);
	kernel_thread(son_thread,"son",CLONE_KERNEL);
	kernel_thread(daughter_thread,"daughter",CLONE_KERNEL);

	init_MUTEX(&mutex);
	init_MUTEX_LOCKED(&apple);
	init_MUTEX_LOCKED(&orange);

	return 0;
}

static void __exit eatingfruit_exit(void)
{
	printk("eating fruit module is leaving..\n");
}

module_init(eatingfruit_init);
module_exit(eatingfruit_exit);
MODULE_LICENSE("GPL");
