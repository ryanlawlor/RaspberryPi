obj-m += ryanfs.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	gcc mkfs.c -g -o mkfs


do: all
	sudo ./mkfs
	sudo insmod ryanfs.ko
	sudo mount -t ryanfs /dev/mmcblk0p3 mount_pt/
	sudo chmod 777 mount_pt/
	ls -lh mount_pt/
	# cat mount_pt/readme.txt

undo:
	sudo umount mount_pt/
	sudo rmmod ryanfs

clean:
	rm -f *.mod.*
	rm -f *.ko
	rm -f *.o
	rm -f -r .tmp_versions
	rm -f -r Module.symvers
	rm -f -r modules.order
	
	
