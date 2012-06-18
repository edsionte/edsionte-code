#include <asm/atomic.h>
#include <linux/cdev.h>

struct mapdrv{
	struct cdev mapdev;
	atomic_t usage;
	/*Since this is read-only, we don't need sem or locks.*/
};

