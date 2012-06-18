obj-m = printfs.o

KERNEL_PATH = /lib/modules/`uname -r`/build
#KERNEL_PATH = /usr/src/linux-2.6.30.5
all:
	make -C $(KERNEL_PATH) M=$(PWD) modules
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
