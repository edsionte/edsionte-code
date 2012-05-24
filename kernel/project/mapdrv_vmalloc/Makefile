obj-m:= map_driver.o
modules:
	make -C /lib/modules/`uname -r`/build M=`pwd` modules

install:
	chmod +x ./install.sh
	./install.sh
uninstall:
	rm -f /dev/mapdrv0
	/sbin/rmmod map_driver
test: maptest.c
	gcc -Wall -o maptest maptest.c
clean:
	rm -rf maptest
	rm -rf *.o *.ko *.mod.c .tmp_versions .*.cmd Module.symvers
