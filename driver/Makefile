#obj-m := hellokernel.o
obj-m := kernumbers.o

all :
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` modules
clean :
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` clean
