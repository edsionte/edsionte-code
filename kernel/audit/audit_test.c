#include <stdio.h>
#include <errno.h>
#include <sys/syscall.h>
#include <linux/types.h>
#include <unistd.h>

#define COMM_SIZE 16
#define __NR_audit 339

struct syscall_buf {
	__u32 serial;
	__u32 ts_sec;
	__u32 ts_micro;
	__u32 syscall;
	__u32 status;
	pid_t pid;
	uid_t uid;
	__u8 comm[COMM_SIZE];
};

#define AUDIT_BUF_SIZE  (100 * sizeof(struct syscall_buf))
//_syscall4(int, audit, __u8, type, __u8 *, us_buf, __u16, us_buf_size, __u8, reset);

int main()
{
	__u8 log_buf[AUDIT_BUF_SIZE];
	unsigned char reset = 1;
	int num = 0;
	struct syscall_buf *p = NULL;
	
	while (1) {
//		num = audit(0, log_buf, AUDIT_BUF_SIZE, reset);
		num = syscall(__NR_audit, 0, log_buf, AUDIT_BUF_SIZE, reset);
		printf("The num of log in buffer:%d\n", num);
		int i;
		p = (struct syscall_buf *)log_buf;
		for ( i = 0; i < num; i++) {
			printf("num[%d],serial:%d\t", i, p[i].serial);
			printf("syscall:%d,status:%d\n", p[i].syscall, p[i].status);
			printf("ts_sec:%d,pid:%d,uid:%d,comm:%s\n", p[i].ts_sec, p[i].pid, p[i].uid, p[i].comm);
		}
	}

	return 1;
}
