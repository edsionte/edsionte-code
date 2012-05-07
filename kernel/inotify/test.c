#include <stdio.h>
#include <sys/inotify.h>

int main()
{
	printf("sizeof(struct inotify_evetn) = %d\n", sizeof(struct inotify_event));
	return 0;
}
