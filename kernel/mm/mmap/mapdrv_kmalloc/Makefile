obj-m:= mapdrv_kmalloc.o
modules:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

install:
	chmod +x ./install.sh
	./install.sh
uninstall:
	rm -f /dev/mapdrv_k
	/sbin/rmmod mapdrv_kmalloc
test: maptest.c
	gcc -Wall -o maptest maptest.c
clean:
	rm -rf maptest
	rm -rf *.o *.ko *.mod.c .tmp_versions .*.cmd Module.symvers
