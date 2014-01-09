obj-m += gpio.o
KDIR = /usr/src/kernel
PWD := $(shell pwd)

.PHONY:
all: kernel repeat message

kernel:
	make -C $(KDIR) M=$(PWD) modules

repeat: repeat.c
	gcc -o repeat repeat.c

message: send_message.c
	gcc -o send_message send_message.c

clean:
	make -C $(KDIR) M=$(PWD) clean
