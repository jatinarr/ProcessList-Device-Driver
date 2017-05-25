obj-m:=process_list.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	gcc userproc.c -o userproc
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	find . -type f -executable -delete
