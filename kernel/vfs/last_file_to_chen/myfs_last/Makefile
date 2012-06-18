obj-m += myfs.o cr_file.o
kernelpath = /lib/modules/`uname -r`/build
all:
	make -C $(kernelpath)  M=$(PWD) modules

install:
	insmod myfs.ko
	insmod cr_file.ko
	mount -t myfs myfs /mnt
	ls /mnt

clean:
	make -C $(kernelpath)  M=$(PWD) clean 
