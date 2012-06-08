#include <linux/init.h>
#include <linux/module.h>
#include <linux/wait.h> //wait_queue_head_t
#include <linux/sched.h> //TASK_INTERRUPTIBEL
#include <asm/current.h> //current
#include <asm/uaccess.h> //copy_to_user
#include <linux/time.h>

#define COMM_SIZE 16
struct syscall_buf {
	u32 serial;
	u32 ts_sec;
	u32 ts_micro;
	u32 syscall;
	u32 status;
	pid_t pid;
	uid_t uid;
	uid_t euid;
	u8 comm[COMM_SIZE];
};

#define AUDIT_BUF_SIZE 100 * sizeof(struct syscall_buf)

DECLARE_WAIT_QUEUE_HEAD(buffer_wait);

static struct syscall_buf audit_buf[AUDIT_BUF_SIZE];//缓冲区
static int current_pos = 0;//缓冲区位置
static u32 serial = 0;//序列号

extern void(*my_getaudit)(int, int);
extern int(*my_sys_audit)(unsigned char, unsigned char *, unsigned short, unsigned char);

//内核函数
void do_audit(int syscall, int return_status)
{
	struct syscall_buf *ppb_temp;
	struct timespec now;

	if (current_pos < AUDIT_BUF_SIZE) {
		ppb_temp = &audit_buf[current_pos];

		ppb_temp->serial = serial++;

		now = current_kernel_time();
		ppb_temp->ts_sec = now.tv_sec;
		ppb_temp->ts_micro = now.tv_nsec;

		ppb_temp->syscall = syscall;
		ppb_temp->pid = current->pid;
		ppb_temp->uid = current->cred->uid;
		ppb_temp->euid = current->cred->euid;

		memcpy(ppb_temp->comm, current->comm, COMM_SIZE);

		if (++current_pos == AUDIT_BUF_SIZE * 8 / 10) {
			printk("In module_audit: yes, it near full!\n");
			wake_up_interruptible(&buffer_wait);
		}
	}
}

//audit系统调用服务历程
int audit_func(u8 type, u8 *us_buf, u16 us_buf_size, u8 reset) 
{
	int ret = 0;
	if (!type) {
		if (__clear_user(us_buf, us_buf_size)) {
			printk("Error:clear_user\n");
			return 0;
		}

		printk("In module_systemcall:starting..\n");
		ret = wait_event_interruptible(buffer_wait, current_pos >= AUDIT_BUF_SIZE * 8 / 10);
		printk("In module_systemcall:over, current_pos is %d\n", current_pos);

		if (__copy_to_user(us_buf, audit_buf, (current_pos) * sizeof(struct syscall_buf))) {
			printk("Error:copy error\n");
			return 0;
		}

		ret = current_pos;
		current_pos = 0;
	}

	return ret;
}

static int __init audit_init(void)
{
	my_sys_audit = audit_func;
	my_getaudit = do_audit;

	printk("Starting system call auditing\n");

	return 0;
}

static void __exit audit_exit(void)
{
	my_getaudit = NULL;
	my_sys_audit = NULL;

	printk("Exiting system call auditing\n");
}

module_init(audit_init);
module_exit(audit_exit);

