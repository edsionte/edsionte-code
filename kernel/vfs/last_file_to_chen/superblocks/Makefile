obj-m = printsb.o

KERNEL_PATH = /lib/modules/`uname -r`/build
all:
	make -C $(KERNEL_PATH) M=$(PWD) modules
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
