CC	= gcc
LD 	= ld  
AS	= as

all:
	make -C ./src
	make -C ./drivers
	make -C ./boot all
	
	$(LD) -T janus86.lds  -o janus86.bin \
		boot/start.o boot/setup.o \
		src/kernel/janus86.o src/kernel/x86.o \
		src/mm/pageinit.o src/mm/kmalloc.o \
		src/kernel/bootinfo.o src/kernel/kprintf.o src/kernel/fs.o \
		src/fs/ext2/superblock.o src/fs/ext2/ext2.o src/fs/ext2/dir.o  \
		src/utils/shell.o src/utils/shell_builtin.o \
		drivers/video.o drivers/keyboard.o drivers/hd.o drivers/diskio.o drivers/8259.o \
	 	drivers/pit.o drivers/serial.o \
		src/kernel/delay.o src/kernel/idt.o src/kernel/string.o \

	cat boot/bootsect.bin janus86.bin boot/a.img > janus86.img

clean:
	make -C ./include clean
	make -C ./src 	clean
	make -C ./drivers clean
	make -C ./boot	clean
	rm -rf *~

install:
	dd if=janus86.img of=/dev/fd0
