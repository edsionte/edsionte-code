#define COMM_SIZE 16
struct syscall_buf {
	u32 serial;
	u32 ts_sec;
	u32 ts_micro;
	u32 syscall;
	u32 status;
	pid_t pid;
	uid_t uid;
	u8 comm[COMM_SIZE];
}

#define AUDIT_BUF_SIZE 100
